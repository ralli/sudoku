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

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "sudokumodel.hpp"
#include "../sudoku/gridchecker.hpp"

void SudokuModel::load(std::string &filename) {
    std::ifstream in(filename.c_str());

    if (!in) {
        throw std::runtime_error("cannot open file");
    }

    Grid copy;
    copy.load(in);
    selected_cell = 0;
    grid = copy;
    m_signal_changed.emit();
}

void SudokuModel::load_from_string(const std::string &s) {
    std::istringstream in(s);
    Grid copy;
    copy.load(in);
    selected_cell = 0;
    grid = copy;
    m_signal_changed.emit();
}

void SudokuModel::clear() {
    grid.init_cells();
    selected_cell = 0;
    m_signal_changed.emit();
}

SudokuModel::SudokuModel() :
    selected_cell(0) {
}

int SudokuModel::get_selected_cell() const {
    return selected_cell;
}

void SudokuModel::set_selected_cell(int selected_cell) {
    this->selected_cell = selected_cell;
    m_signal_changed.emit();
}

void SudokuModel::set_current_cell_value(int value) {
    Cell &cell = grid[selected_cell];
    if (cell.has_value())
        grid.clear_cell_value(grid[selected_cell]);
    cell.set_value(value);
    grid.cleanup_choice(cell);
    m_signal_changed.emit();
}

void SudokuModel::toggle_current_cell_choice(int value) {
    Cell &cell = grid[selected_cell];
    if (cell.has_value())
        return;
    if(cell.has_choice(value))
        cell.remove_choice(value);
    else
        cell.add_choice(value);
    m_signal_changed.emit();
}

void SudokuModel::clear_current_cell_value() {
    grid.clear_cell_value(grid[selected_cell]);
    m_signal_changed.emit();
}

void SudokuModel::move_selection_up() {
    int row = selected_cell / 9;
    int col = selected_cell % 9;
    row = (row == 0) ? 8 : row - 1;
    selected_cell = row * 9 + col;
    m_signal_changed.emit();
}

void SudokuModel::move_selection_down() {
    int row = selected_cell / 9;
    int col = selected_cell % 9;
    row = (row == 8) ? 0 : row + 1;
    selected_cell = row * 9 + col;
    m_signal_changed.emit();
}

void SudokuModel::move_selection_left() {
    int row = selected_cell / 9;
    int col = selected_cell % 9;
    col = (col == 0) ? 8 : col - 1;
    selected_cell = row * 9 + col;
    m_signal_changed.emit();
}

void SudokuModel::move_selection_right() {
    int row = selected_cell / 9;
    int col = selected_cell % 9;
    col = (col == 8) ? 0 : col + 1;
    selected_cell = row * 9 + col;
    m_signal_changed.emit();
}

bool SudokuModel::is_cell_selected(int row, int col) const {
    int idx = row * 9 + col;
    return selected_cell == idx;
}

SudokuModel::type_signal_changed &SudokuModel::signal_changed() {
    return m_signal_changed;
}

bool SudokuModel::has_value(int idx) const {
    return grid[idx].has_value();
}

int SudokuModel::get_value(int idx) const {
    return grid[idx].get_value();
}

bool SudokuModel::has_choice(int idx, int value) const {
    return grid[idx].has_choice(value);
}

void SudokuModel::print(std::ostream &out) const {
    grid.println(out);
}

bool SudokuModel::check() const {
    GridChecker checker;
    return checker.check(grid);
}
