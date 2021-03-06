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

#include <vector>
#include <set>
#include <iostream>

#include "pointing.hpp"
#include "grid.hpp"
#include "range.hpp"
#include "hintconsumer.hpp"

PointingHint::PointingHint(Grid &grid, int start_row, int start_col, int value,
        int row, int col) :
    grid(grid), start_row(start_row), start_col(start_col), value(value), row(
            row), col(col) {
}

void PointingHint::print_description(std::ostream &out) const {
    out << "pointing: ";
    out << "block(" << start_row + 1 << "," << start_col + 1 << ") ";
    out << "value: " << value;
    if (is_row_hint()) {
        out << " row: " << row + 1;
    } else {
        out << " column: " << col + 1;
    }
    out << " removing: " << print_choices_to_remove(get_choices_to_remove());
}

const char *PointingHint::get_name() const {
    return "Pointing";
}

bool PointingHint::is_row_hint() const {
    return row != -1;
}

bool PointingHint::is_col_hint() const {
    return col != -1;
}

void PointingHintProducer::find_block_hint(int block_idx, Grid &grid,
        HintConsumer &consumer) const {
    int start_row = (block_idx / 3) * 3;
    int start_col = (block_idx % 3) * 3;
    std::vector<std::set<int> > value_rows(10);
    std::vector<std::set<int> > value_cols(10);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int idx = (start_row + row) * 9 + (start_col + col);
            Cell &cell = grid[idx];
            if (!cell.has_value()) {
                for (int value = 1; value < 10; ++value) {
                    if (cell.has_choice(value)) {
                        value_rows[value].insert(start_row + row);
                    }
                }
            }
        }
    }

    for (int value = 1; value < 10; ++value) {
        if (value_rows[value].size() == 1) {
            int row = *value_rows[value].begin();
            for (int col = 0; col < 9; ++col) {
                int idx = row * 9 + col;
                Cell &cell = grid[idx];
                if (!cell.has_value() && (col < start_col || col >= start_col
                        + 3) && cell.has_choice(value)) {
                    if (!consumer.consume_hint(create_hint(grid, start_row,
                            start_col, value, row, -1)))
                        return;
                }
            }
        }
    }

    for (int col = 0; col < 3; ++col) {
        for (int row = 0; row < 3; ++row) {
            int idx = (start_row + row) * 9 + (start_col + col);
            Cell &cell = grid[idx];

            if (!cell.has_value()) {
                for (int value = 1; value < 10; ++value) {
                    if (cell.has_choice(value)) {
                        value_cols[value].insert(start_col + col);
                    }
                }
            }
        }
    }

    for (int value = 1; value < 10; ++value) {
        if (value_cols[value].size() == 1) {
            int col = *value_cols[value].begin();
            for (int row = 0; row < 9; ++row) {
                if (row < start_row || row >= start_row + 3) {
                    int idx = 9 * row + col;
                    Cell cell = grid[idx];
                    if (!cell.has_value() && cell.has_choice(value)) {
                        if (!consumer.consume_hint(create_hint(grid, start_row,
                                start_col, value, -1, col)))
                            return;
                    }
                }
            }
        }
    }
}

void PointingHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    for (int block_idx = 0; block_idx < 9; ++block_idx) {
        find_block_hint(block_idx, grid, consumer);
        if (!consumer.wants_more_hints())
            return;
    }
}

PointingHint *PointingHintProducer::create_hint(Grid &grid, int start_row,
        int start_col, int value, int row, int col) const {
    PointingHint *hint = new PointingHint(grid, start_row, start_col, value,
            row, col);
    if (hint->is_row_hint()) {
        int r = row;
        for (int c = 0; c < 9; ++c) {
            if (c < start_col || c >= start_col + 3) {
                int idx = 9 * r + c;
                if (grid[idx].has_choice(value))
                    hint->add_choice_to_remove(&grid[idx], value);
            }
        }
    } else {

        int c = col;

        for (int r = 0; r < 9; ++r) {
            if (r < start_row || r >= start_row + 3) {
                int idx = 9 * r + c;
                if (grid[idx].has_choice(value))
                    hint->add_choice_to_remove(&grid[idx], value);
            }
        }

    }
    return hint;
}
