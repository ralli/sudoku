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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "gtkmm.h"
#include "sudokuview.hpp"
#include "statusview.hpp"
#include "hintview.hpp"
#include "sudokumodel.hpp"

class MainWindow: public Gtk::Window {
private:
public:
    MainWindow(const Glib::RefPtr<SudokuModel> &model);
    void on_file_new();
    void on_file_open();
    void on_file_exit();
    void on_edit_undo();
    void on_edit_redo();
    void on_model_changed();
    void on_edit_copy();
    void on_edit_paste();
    void on_edit_difficulty_easy();
    void on_edit_difficulty_medium();
    void on_edit_difficulty_hard();
    void on_file_check();
    void on_highlight_nothing();
    void on_highlight_one();
    void on_highlight_two();
    void on_highlight_three();
    void on_highlight_four();
    void on_highlight_five();
    void on_highlight_six();
    void on_highlight_seven();
    void on_highlight_eight();
    void on_highlight_nine();
    void on_help_about();
    void on_view_choices();
    void on_view_sidebar();
private:
    void init_actions();
    void init_file_actions();
    void init_edit_actions();
    void init_view_actions();
    void init_view_highlight_actions();
    void init_help_actions();
    void on_clipboard_text_received(const Glib::ustring& text);
    void update_statusbar();
private:
    Gtk::VBox m_box;
    Gtk::HBox m_hbox;
    Glib::RefPtr<Gtk::UIManager> m_refUIManager;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
    Glib::RefPtr<Gtk::Action> m_actionUndo;
    Glib::RefPtr<Gtk::Action> m_actionRedo;
    Glib::RefPtr<Gtk::RadioAction> m_difficultyEasy;
    Glib::RefPtr<Gtk::RadioAction> m_difficultyMedium;
    Glib::RefPtr<Gtk::RadioAction> m_difficultyHard;
    Gtk::Statusbar m_statusbar;
    Glib::RefPtr<SudokuModel> model;
    SudokuView sudokuView;
    StatusView statusView;
    HintView hintView;
    guint context_id_status;
};

#endif
