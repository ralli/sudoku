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

#include "grid.hpp"
#include "boxlinereduction.hpp"
#include "hintconsumer.hpp"
#include "util.hpp"

BoxLineReductionHint::BoxLineReductionHint(int value,
        const std::vector<int> &cells_to_clear, const Range &block1,
        const Range &block2, const Range &block3, const Range &range1,
        const Range &range2, const Range &range3) :
    value(value), cells_to_clear(cells_to_clear), block1(block1),
            block2(block2), block3(block3), range1(range1), range2(range2),
            range3(range3) {

}

void BoxLineReductionHint::apply(Grid &grid) {
    for (std::vector<int>::const_iterator i = cells_to_clear.begin(); i
            != cells_to_clear.end(); ++i) {
        Cell &cell = grid[*i];
        cell.remove_choice(value);
    }
}

void BoxLineReductionHint::print_description(std::ostream &out) const {
    out << "box line reduction: value=" << value << ", block1="
            << block1.get_name() << ", block2=" << block2.get_name()
            << ", block to clear=" << block3.get_name()
            << " ranges to clear in " << block3.get_name() << ": "
            << range1.get_name() << ", " << range2.get_name()
            << ". range to keep: " << range3.get_name();
    out << " clearing cells:";
    for (std::vector<int>::const_iterator i = cells_to_clear.begin(); i
            != cells_to_clear.end(); ++i) {
        out << ' ' << print_row_col(*i);
    }
}

const char *BoxLineReductionHint::get_name() const {
    return "box line reduction";
}

void BoxLineReductionHintProducer::find_hints(Grid &grid,
        HintConsumer &consumer) {
    for (int value = 1; value < 10; ++value) {
        for (int i = 0; i < 3; ++i) {
            find_line_hints(RANGES.get_rows(), value, 3 * i, grid, consumer);
            if (!consumer.wants_more_hints())
                return;
        }

        for (int i = 0; i < 3; ++i) {
            find_line_hints(RANGES.get_columns(), value, 3 * i, grid, consumer);
            if (!consumer.wants_more_hints())
                return;
        }
    }
}

struct print_bitset {
    std::bitset<3> &s;

    print_bitset(std::bitset<3> &s) :
        s(s) {
    }

    void print(std::ostream &out) const {
        for (int i = 0; i < 3; ++i)
            out << s[i];
    }
};

inline std::ostream &operator <<(std::ostream &out, const print_bitset &ps) {
    ps.print(out);
    return out;
}

void BoxLineReductionHintProducer::find_line_hints(
        const std::vector<Range> &ranges, int value, int start, Grid &grid,
        HintConsumer &consumer) const {
    std::vector<std::bitset<3> > positions(3); // first dimension block within range, second dimension row/col

    for (int row = 0; row < 3; ++row) {
        const Range &r = ranges[row + start];
        for (int col = 0; col < 9; ++col) {
            Cell &cell = grid[r[col]];
            if (cell.has_choice(value)) {
                int block_idx = col / 3; // 0 <= idx <= 2
                positions[block_idx][row] = 1;
            }
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = i + 1; j < 3; ++j) {
            std::bitset<3> ijrows = positions[i] | positions[j];

            /*
             * ijrows.count() == 2 <==> block[i] and block[j] have two
             * rows in common
             */
            if (ijrows.count() == 2 && (positions[i] & positions[j]).any()) {
                for (int k = 0; k < 3; ++k) {
                    if (k != i && k != j) {
                        /*
                         * positions[k].count() > 1 means that there is at least
                         * one cell to clear since one row must be occupied by
                         * the remaining row of block[i] and block[j]
                         * and the other contains a choice that will be cleared
                         */
                        if ((positions[k] & ~ijrows).any()) {
                            int rows[3];
                            get_row_indexes(ijrows, rows);
                            Hint *hint = create_hint(grid, ranges, value,
                                    start, i, j, k, rows);
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

void BoxLineReductionHintProducer::get_row_indexes(
        const std::bitset<3> &ijrows, int rows[3]) const {
    int idx = 0;
    int missing = -1;

    for (int i = 0; i < 3; ++i) {
        if (ijrows[i]) {
            rows[idx++] = i;
        } else {
            missing = i;
        }
    }

    rows[idx++] = missing;
}

void BoxLineReductionHintProducer::get_common_fields(const Range &range1,
        const Range &range2, std::vector<int> &fields) const {
    bool flags[81];

    std::fill(flags, flags + 81, false);

    for (int i = 0; i < 9; ++i) {
        flags[range1[i]] = true;
    }

    for (int i = 0; i < 9; ++i) {
        if (flags[range2[i]]) {
            fields.push_back(range2[i]);
        }
    }
}

Hint *BoxLineReductionHintProducer::create_hint(Grid &grid, const std::vector<
        Range> &ranges, int value, int start, int i1, int i2, int i3,
        int rows[3]) const {
    const Range &range1 = ranges[rows[0] + start];
    const Range &range2 = ranges[rows[1] + start];
    const Range &range3 = ranges[rows[2] + start];
    const Range &block1 =
            RANGES.get_block(grid[range1[i1 * 3]].get_block_idx());
    const Range &block2 =
            RANGES.get_block(grid[range1[i2 * 3]].get_block_idx());
    const Range &block3 =
            RANGES.get_block(grid[range1[i3 * 3]].get_block_idx());

    std::vector<int> fields;
    get_common_fields(range1, block3, fields);
    get_common_fields(range2, block3, fields);

    std::vector<int> cells_to_clear;
    for (std::vector<int>::const_iterator i = fields.begin(); i != fields.end(); ++i) {
        if (grid[*i].has_choice(value)) {
            cells_to_clear.push_back(*i);
        }
    }

    if (cells_to_clear.empty())
        return 0;

    return new BoxLineReductionHint(value, cells_to_clear, block1, block2,
            block3, range1, range2, range3);
}
