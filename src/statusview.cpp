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

#if !_MSC_VER
#include "../include/config.h"
#endif
#include "../include/gettext.h"
#include <gtkmm.h>
#include <sstream>
#include "statusview.hpp"

#define _(X) gettext(X)

StatusView::StatusView(const Glib::RefPtr<SudokuModel> &model) :
    model(model), m_todo_label("0"), m_done_label("0"), m_choices_label("0") {
    Gtk::Frame *frame = manage(new Gtk::Frame());
    pack_end(*frame, true, true, 5);
    frame->set_label(_("Status"));
    Gtk::Table *table = Gtk::manage(new Gtk::Table(3, 2, true));
    frame->add(*table);
    Gtk::Label *label = Gtk::manage(new Gtk::Label(_("To-Do:")));
    label->set_alignment(Gtk::ALIGN_LEFT);
    table->attach(*label, 0, 1, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 5,
            5);
    table->attach(m_todo_label, 1, 2, 0, 1, Gtk::FILL | Gtk::EXPAND,
            Gtk::SHRINK, 5, 5);

    label = Gtk::manage(new Gtk::Label(_("Done:")));
    label->set_alignment(Gtk::ALIGN_LEFT);
    table->attach(*label, 0, 1, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 5,
            5);
    table->attach(m_done_label, 1, 2, 1, 2, Gtk::FILL | Gtk::EXPAND,
            Gtk::SHRINK, 5, 5);

    label = Gtk::manage(new Gtk::Label(_("Choices:")));
    label->set_alignment(Gtk::ALIGN_LEFT);
    table->attach(*label, 0, 1, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 5, 5);
    table->attach(m_choices_label, 1, 2, 2, 3, Gtk::FILL | Gtk::EXPAND,
            Gtk::SHRINK, 5, 5);

    m_choices_label.set_alignment(Gtk::ALIGN_LEFT);
    m_todo_label.set_alignment(Gtk::ALIGN_LEFT);
    m_done_label.set_alignment(Gtk::ALIGN_LEFT);

    model->signal_changed().connect(sigc::mem_fun(this,
            &StatusView::on_model_changed_event));

    on_model_changed_event();
}

inline Glib::ustring to_s(int i) {
    std::ostringstream os;
    os << i;
    return os.str();
}

void StatusView::on_model_changed_event() {
    int todo = model->get_todo();
    m_todo_label.set_label(to_s(todo));
    m_done_label.set_label(to_s(81 - todo));
    m_choices_label.set_label(to_s(model->get_num_choices()));
}
