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

#ifndef SUDOKUMODEL_HPP_
#define SUDOKUMODEL_HPP_

#include "gtkmm.h"
#include <string>
#include "../sudoku/grid.hpp"

#include "commands.hpp"

class SudokuModel: public Glib::Object {
public:
    /*! \brief a signal with no arguments returning void */
    typedef sigc::signal<void> type_signal_changed;
    /*! \brief the difficulty level of a sudoku */
    typedef enum {
        EASY, MEDIUM, HARD
    } DifficultyLevel;
private:
    int selected_cell;
    int highlighted_choice;

    type_signal_changed m_signal_changed;
    Grid grid;
    UndoManager undo_manager;
    DifficultyLevel difficulty_level;
public:
    SudokuModel();

    void load(std::string &filename);
    void load_from_string(const std::string &s);
    void clear();
    void generate();

    bool can_undo() const;
    bool can_redo() const;
    void undo();
    void redo();

    bool has_value(int idx) const;
    int get_value(int idx) const;
    bool has_choice(int idx, int value) const;

    int get_selected_cell() const;
    bool is_cell_selected(int row, int col) const;
    void set_selected_cell(int selected_cell);

    int get_highlighted_choice() const;
    void set_highlighted_choice(int highlighted_choice);

    int get_todo() const;
    int get_num_choices() const;

    DifficultyLevel get_difficulty_level() const;
    void set_difficulty_level(DifficultyLevel difficulty_level);

    void move_selection_up();
    void move_selection_down();
    void move_selection_left();
    void move_selection_right();
    void toggle_current_cell_choice(int value);
    void set_current_cell_value(int value);
    void clear_current_cell_value();
    type_signal_changed &signal_changed();
    void print(std::ostream &out) const;
    bool check() const;
private:
    SudokuModel(const SudokuModel &other) {
    }
    SudokuModel &operator =(const SudokuModel &other) {
        return *this;
    }
};

#endif /* SUDOKUMODEL_HPP_ */
