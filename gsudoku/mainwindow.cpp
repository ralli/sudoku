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

#include "mainwindow.hpp"
#include "gettext.h"
#include <iostream>

#define _(X) gettext(X)

MainWindow::MainWindow(const Glib::RefPtr<SudokuModel> &model) :
    model(model), sudokuView(model) {
    set_title(_("Sudoku"));
    set_default_size(800, 600);
    add(m_box);

    m_refActionGroup = Gtk::ActionGroup::create();

    m_refActionGroup->add(Gtk::Action::create("FileMenu", _("_File")));

    m_refActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW,
            _("_New"), _("Generates a new mountain")), sigc::mem_fun(
            *this, &MainWindow::on_file_new));

    m_refActionGroup->add(Gtk::Action::create("FileExit", Gtk::Stock::QUIT,
            _("E_xit"), _("Exits the application")), sigc::mem_fun(*this,
            &MainWindow::on_file_exit));

    m_refUIManager = Gtk::UIManager::create();
    m_refUIManager->insert_action_group(m_refActionGroup);

    add_accel_group(m_refUIManager->get_accel_group());

    try {

        Glib::ustring ui_info = "<ui>"
            "  <menubar name='MenuBar'>"
            "    <menu action='FileMenu'>"
            "      <menuitem action='FileNew' />"
            "      <separator />"
            "      <menuitem action='FileExit' />"
            "    </menu>"
            "  </menubar>"
            "  <toolbar  name='ToolBar'>"
            "    <toolitem action='FileNew'/>"
            "  </toolbar>"
            "</ui>";

        m_refUIManager->add_ui_from_string(ui_info);
    }

    catch (const Glib::Error &ex) {
        std::cerr << "Scheisse: " << ex.what() << std::endl;
    }

    Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
    if (pMenubar)
        m_box.pack_start(*pMenubar, Gtk::PACK_SHRINK);

    Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar");
    if (pToolbar)
        m_box.pack_start(*pToolbar, Gtk::PACK_SHRINK);

    m_box.pack_start(sudokuView, Gtk::PACK_EXPAND_WIDGET);
    m_box.pack_end(m_statusbar, Gtk::PACK_SHRINK);

    show_all_children();
}

void MainWindow::on_file_new() {

}

void MainWindow::on_file_exit() {
    hide();
}
