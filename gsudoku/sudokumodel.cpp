#include <fstream>
#include <stdexcept>
#include "sudokumodel.hpp"

void SudokuModel::load(std::string &filename) {
    std::ifstream in(filename.c_str());

    if (!in) {
        throw std::runtime_error("cannot open file");
    }

    grid.load(in);
    m_signal_changed.emit();
}

void SudokuModel::clear() {
    grid.init_cells();
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
