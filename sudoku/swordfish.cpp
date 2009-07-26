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
#include <vector>
#include <bitset>

#include "swordfish.hpp"
#include "util.hpp"
#include "range.hpp"
#include "grid.hpp"
#include "hintconsumer.hpp"

SwordfishHint::SwordfishHint(int value,
        const std::vector<const Range *> &row_ranges, const std::vector<
                const Range *> &col_ranges) :
    value(value), row_ranges(row_ranges), col_ranges(col_ranges) {

}

void SwordfishHint::print_description(std::ostream &out) const {
    out << "swordfish: value: " << value << " first dimension: ";
    for (std::vector<const Range *>::const_iterator i = row_ranges.begin(); i
            != row_ranges.end(); ++i)
        out << (*i)->get_name() << ' ';
    out << "second dimension: ";
    for (std::vector<const Range *>::const_iterator i = col_ranges.begin(); i
            != col_ranges.end(); ++i)
        out << (*i)->get_name() << ' ';
    out << " removing: " << print_choices_to_remove(get_choices_to_remove());
}

const char *SwordfishHint::get_name() const {
    return "Swordfish";
}

void SwordfishHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    for (int value = 1; value < 10; ++value) {
        find_swordfishes(value, grid, RANGES.get_rows(), RANGES.get_columns(),
                consumer);
        if (!consumer.wants_more_hints())
            return;
        find_swordfishes(value, grid, RANGES.get_columns(), RANGES.get_rows(),
                consumer);
        if (!consumer.wants_more_hints())
            return;
    }
}

void SwordfishHintProducer::find_swordfishes(int value, Grid &grid,
        const std::vector<Range> &row_ranges,
        const std::vector<Range> &col_ranges, HintConsumer &consumer) const {
    std::vector<std::bitset<9> > rowsets(9);

    fill_rowsets(value, grid, row_ranges, rowsets);

    for (int i = 0; i < 9; ++i) {
        if (rowsets[i].count() > 1) {
            for (int j = i + 1; j < 9; ++j) {
                if (rowsets[j].count() > 1) {
                    for (int k = 0; k < 9; ++k) {
                        if (k != i && k != j && rowsets[k].count() > 1) {
                            std::bitset<9> x = rowsets[i];
                            x |= rowsets[j];
                            x |= rowsets[k];
                            if (x.count() == 3) {
                                SwordfishHint *hint = create_swordfish_hint(
                                        value, i, j, k, x, row_ranges,
                                        col_ranges, grid);
                                if (hint) {
                                    if (!consumer.consume_hint(hint))
                                        return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

SwordfishHint *SwordfishHintProducer::create_swordfish_hint(int value,
        int row1, int row2, int row3, const std::bitset<9> &columnset,
        const std::vector<Range> &ranges,
        const std::vector<Range> &secondary_ranges, Grid &grid) const {
    std::vector<int> cols;
    std::vector<int> cells_to_clear;

    for (int i = 0; i < 9; ++i) {
        if (columnset[i]) {
            cols.push_back(i);
        }
    }

    for (int row = 0; row < 9; ++row) {
        if (row != row1 && row != row2 && row != row3) {
            for (int colidx = 0; colidx < 3; ++colidx) {
                int col = cols[colidx];
                if (grid[ranges[row][col]].has_choice(value)) {
                    cells_to_clear.push_back(ranges[row][col]);
                }
            }
        }
    }

    if (cells_to_clear.empty())
        return 0;

    std::vector<const Range *> row_ranges;
    row_ranges.push_back(&ranges[row1]);
    row_ranges.push_back(&ranges[row2]);
    row_ranges.push_back(&ranges[row3]);
    std::vector<const Range *> col_ranges;
    col_ranges.push_back(&secondary_ranges[cols[0]]);
    col_ranges.push_back(&secondary_ranges[cols[1]]);
    col_ranges.push_back(&secondary_ranges[cols[2]]);

    return create_hint(grid, value, row_ranges, col_ranges, cells_to_clear);
}

void SwordfishHintProducer::fill_rowsets(int value, Grid &grid,
        const std::vector<Range> &ranges, std::vector<std::bitset<9> > &bitsets) const {
    for (int i = 0; i < 9; ++i) {
        const Range &range = ranges[i];
        for (int j = 0; j < 9; ++j) {
            bitsets[i][j] = grid[range[j]].has_choice(value);
        }
    }
}

SwordfishHint *SwordfishHintProducer::create_hint(Grid &grid, int value,
        const std::vector<const Range *> &row_ranges, const std::vector<
                const Range *> &col_ranges,
        const std::vector<int> &cells_to_clear) const {
    SwordfishHint *hint = new SwordfishHint(value, row_ranges, col_ranges);
    for (std::vector<int>::const_iterator i = cells_to_clear.begin(); i
            != cells_to_clear.end(); ++i) {
        hint->add_choice_to_remove(&grid[(*i)], value);
    }
    return hint;
}
