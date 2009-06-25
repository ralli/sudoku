#include "mainwindow.hpp"
#include "gettext.h"
#include <iostream>

#define _(X) gettext(X)

MainWindow::MainWindow() {
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
