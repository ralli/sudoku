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

class SudokuModel : public Glib::Object {
public:
    /*! \brief a signal with no arguments returning void */
    typedef sigc::signal<void> type_signal_changed;
private:
    int selected_cell;
    type_signal_changed m_signal_changed;
public:
    SudokuModel();
private:
    SudokuModel(const SudokuModel &other) {
    }
    SudokuModel &operator =(const SudokuModel &other) {
        return *this;
    }
    int get_selected_cell() const;
    bool is_cell_selected(int row, int col) const;
    void set_selected_cell(int selected_cell);
    void move_selection_up();
    void move_selection_down();
    void move_selection_left();
    void move_selection_right();
    type_signal_changed &signal_changed();
};

inline SudokuModel::SudokuModel() :
    selected_cell(0) {
}

inline int SudokuModel::get_selected_cell() const {
    return selected_cell;
}

inline void SudokuModel::set_selected_cell(int selected_cell) {
    this->selected_cell = selected_cell;
    ;
}

inline void SudokuModel::move_selection_up() {
    int row = selected_cell / 9;
    int col = selected_cell % 9;
    row = (row == 0) ? 8 : row - 1;
    selected_cell = row * 9 + col;
}

inline void SudokuModel::move_selection_down() {
    int row = selected_cell / 9;
    int col = selected_cell % 9;
    row = (row == 8) ? 0 : row + 1;
    selected_cell = row * 9 + col;
}

inline void SudokuModel::move_selection_left() {
    int row = selected_cell / 9;
    int col = selected_cell % 9;
    col = (col == 0) ? 8 : col - 1;
    selected_cell = row * 9 + col;
}

inline void SudokuModel::move_selection_right() {
    int row = selected_cell / 9;
    int col = selected_cell % 9;
    col = (col == 8) ? 0 : col + 1;
    selected_cell = row * 9 + col;
}

inline bool SudokuModel::is_cell_selected(int row, int col) const {
    int idx = row * 9 + col;
    return selected_cell == idx;
}

inline SudokuModel::type_signal_changed &SudokuModel::signal_changed() {
    return m_signal_changed;
}

#endif /* SUDOKUMODEL_HPP_ */
