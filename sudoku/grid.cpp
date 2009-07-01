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

#include "range.hpp"
#include "grid.hpp"

void Cell::print_choices(std::ostream &out) const {
    int count = 0;

    if (has_value()) {
        out << get_value();
        ++count;
    } else {
        for (int i = 1; i < 10; ++i) {
            if (has_choice(i)) {
                out << i;
                ++count;
            }
        }
    }

    for (; count < 10; ++count) {
        out << ' ';
    }
}

void Grid::load(std::istream &in) {
    char ch;
    in.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);

    init_cells();

    for (int i = 0; i < 81; ++i) {
        in >> ch;
        int v = isdigit(ch) ? ch - '0' : 0;
        cells[i].set_value(v);
    }

    cleanup_choices();
}

void Grid::print(std::ostream &out) const {
    int i;
    int row;
    int col;

    i = 0;
    for (row = 0; row < 9; ++row) {
        if (row == 3 || row == 6) {
            out << "---+---+---" << std::endl;
            ;
        }
        for (col = 0; col < 9; ++col) {
            if (col == 3 || col == 6)
                out << '|';
            if (cells[i].has_value())
                out << cells[i].get_value();
            else
                out << '.';
            ++i;
        }
        out << std::endl;
    }
}

void Grid::println(std::ostream &out) const {
    for(int i = 0; i < 81; ++i) {
        const Cell &cell = cells[i];
        if(cell.has_value())
            out << cell.get_value();
        else
            out << '.';
    }
}

void Grid::print_choices(std::ostream &out) const {
    int i;
    int row;
    int col;

    i = 0;
    for (row = 0; row < 9; ++row) {
        for (col = 0; col < 9; ++col) {
            cells[i].print_choices(out);
            ++i;
        }
        out << std::endl;
    }
}

void Grid::print_status(std::ostream &out) const {
    out << "Choices: " << get_num_choices() << " todo: " << get_to_do()
            << std::endl;
}

void Grid::cleanup_choice(Cell &cell) {
    if (!cell.has_value()) {
        return;
    }

    cell.clear_choices();

    RangeList::const_index_iterator begin = RANGES.field_begin(cell.get_idx());
    RangeList::const_index_iterator end = RANGES.field_end(cell.get_idx());

    for (RangeList::const_index_iterator j = begin; j != end; ++j) {
        cells[*j].remove_choice(cell.get_value());
    }
}

void Grid::remove_invalid_cell_choices(Cell &cell) {
    if (cell.has_value()) {
        cell.clear_choices();
        return;
    }

    RangeList::const_index_iterator begin = RANGES.field_begin(cell.get_idx());
    RangeList::const_index_iterator end = RANGES.field_end(cell.get_idx());
    for (RangeList::const_index_iterator j = begin; j != end; ++j) {
        if (cells[*j].has_value()) {
            cell.remove_choice(cells[*j].get_value());
        }
    }
}

void Grid::clear_cell_value(Cell &cell) {
    if (!cell.has_value())
        return;

    int value = cell.get_value();
    cell.set_value(0);
    cell.set_all_choices();
    remove_invalid_cell_choices(cell);

    RangeList::const_index_iterator begin = RANGES.field_begin(cell.get_idx());
    RangeList::const_index_iterator end = RANGES.field_end(cell.get_idx());
    for (RangeList::const_index_iterator j = begin; j != end; ++j) {
        Cell &c = cells[*j];
        if (!c.has_value()) {
            c.add_choice(value);
            remove_invalid_cell_choices(c);
        }
    }
}
