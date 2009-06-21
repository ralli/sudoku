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
#include <bitset>

#include "nakeddouble.hpp"
#include "range.hpp"
#include "hintconsumer.hpp"
#include "grid.hpp"
#include "util.hpp"

NakedDoubleHint::NakedDoubleHint(int first_cell_idx, int second_cell_idx,
        int value1, int value2, const Range &range) :
    first_cell_idx(first_cell_idx), second_cell_idx(second_cell_idx), value1(
            value1), value2(value2), range(range) {
}

void NakedDoubleHint::print_description(std::ostream &out) const {
    out << "naked double: cell1: " << print_row_col(first_cell_idx)
            << " cell2: " << print_row_col(second_cell_idx) << " in range: "
            << range.get_name() << " removing: " << print_choices_to_remove(
            get_choices_to_remove());
}

void fill_bitset(Cell &cell, std::bitset<10> &bits) {
    for (int value = 1; value < 10; ++value) {
        bits[value] = cell.has_choice(value);
    }
}

void fill_values(const Cell &cell, std::vector<int> &values) {
    for (int value = 1; value < 10; ++value) {
        if (cell.has_choice(value))
            values.push_back(value);
    }
}

void NakedDoubleHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    std::vector<std::bitset<10> > bitsets(9);

    for (RangeList::const_iterator irange = RANGES.begin(); irange
            != RANGES.end(); ++irange) {
        for (int i = 0; i < 9; ++i) {
            int idx = (*irange)[i];
            Cell &cell = grid[idx];
            fill_bitset(cell, bitsets[i]);
        }

        for (int i = 0; i < 9; ++i) {
            if (bitsets[i].count() == 2) {
                for (int j = i + 1; j < 9; ++j) {
                    if (bitsets[i] == bitsets[j]) {
                        for (int k = 0; k < 9; ++k) {
                            if (k != i && k != j) {
                                if ((bitsets[i] & bitsets[k]).any()) {
                                    int idx1 = (*irange)[i];
                                    int idx2 = (*irange)[j];
                                    std::vector<int> values;
                                    fill_values(grid[idx1], values);
                                    if (!consumer.consume_hint(create_hint(
                                            grid, idx1, idx2, values[0],
                                            values[1], *irange)))
                                        return;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

NakedDoubleHint *NakedDoubleHintProducer::create_hint(Grid &grid,
        int first_cell_idx, int second_cell_idx, int value1, int value2,
        const Range &range) const {
    NakedDoubleHint *hint = new NakedDoubleHint(first_cell_idx,
            second_cell_idx, value1, value2, range);

    for (int i = 0; i < 9; ++i) {
        if (range[i] != first_cell_idx && range[i] != second_cell_idx) {
            Cell &cell = grid[range[i]];
            if (cell.has_choice(value1)) {
                hint->add_choice_to_remove(&cell, value1);
            }
            if (cell.has_choice(value2)) {
                hint->add_choice_to_remove(&cell, value2);
            }
        }
    }

    return hint;
}
