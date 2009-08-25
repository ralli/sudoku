/*
* Copyright (c) 2009, Ralph Juhnke
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or
* without modification, are permitted provided that the following conditions
* are met:
*
*    * Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*
*    * Redistributions in binary form must reproduce the above copyright
*      notice, this list of conditions and the following disclaimer in the
*      documentation and/or other materials provided with the distribution.
*
*    * Neither the name of "Ralph Juhnke" nor the names of its
*      contributors may be used to endorse or promote products derived from
*      this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <sstream>
#include <gtkmm.h>

#if !_MSC_VER
#include "../include/config.h"
#endif
#include "../include/gettext.h"

#include "hintview.hpp"
#include "hint.hpp"


#define _(x) gettext(x)

HintView::HintView(const Glib::RefPtr<SudokuModel> &model) :
model(model) {
    Gtk::Frame *frame = manage(new Gtk::Frame());
    pack_end(*frame, true, true, 5);
    frame->set_label(_("Hint"));
    hint_label.set_label("Das hier ist ein Test\nUnd noch eine Zeile...");
    hint_label.set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
    Gtk::HBox *hbox = manage(new Gtk::HBox());
    frame->add(*hbox);
    hbox->pack_start(hint_label, Gtk::PACK_EXPAND_WIDGET);
    Gtk::VBox *vbox = manage(new Gtk::VBox());
    hbox->pack_start(*vbox, Gtk::PACK_SHRINK);
    Gtk::Button *find_button = Gtk::manage(new Gtk::Button(_("_Find Hint"), true));
    vbox->pack_start(*find_button, Gtk::PACK_SHRINK);
    Gtk::Button *apply_button = Gtk::manage(new Gtk::Button(_("_Apply Hint"), true));
    vbox->pack_start(*apply_button, Gtk::PACK_SHRINK);
    find_button->signal_clicked().connect(sigc::mem_fun(this,
        &HintView::on_find_hint));
    apply_button->signal_clicked().connect(sigc::mem_fun(this,
        &HintView::on_apply_hint));
    model->signal_changed().connect(sigc::mem_fun(this,
        &HintView::on_model_changed_event));
}

void HintView::on_model_changed_event() {
    if (model->get_current_hint() != 0) {
        std::ostringstream os;
        model->get_current_hint()->print_description(os);
        hint_label.set_label(os.str());
    }
    else {
        hint_label.set_label(_("N/A"));
    }
}

void HintView::on_find_hint() {
    if(!model->check())
        hint_label.set_label(_("Invalid"));
    else
        model->find_next_hint();
}

void HintView::on_apply_hint() {
    model->apply_current_hint();
}
