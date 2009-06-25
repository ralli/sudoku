#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "gtkmm.h"
#include "sudokuview.hpp"

class MainWindow : public Gtk::Window {
public:
    MainWindow();
    void on_file_new();
    void on_file_exit();
private:
     Gtk::VBox                      m_box;
     Glib::RefPtr<Gtk::UIManager>   m_refUIManager;
     Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
     Gtk::Statusbar                 m_statusbar;
     SudokuView                     sudokuView;
};

#endif
