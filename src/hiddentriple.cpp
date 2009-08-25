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
#include <set>
#include <bitset>

#include "hiddentriple.hpp"
#include "hintconsumer.hpp"
#include "range.hpp"
#include "util.hpp"
#include "grid.hpp"

HiddenTripleHint::HiddenTripleHint(const std::vector<Cell *> &cells,
        int value1, int value2, int value3, const Range &range) :
    cells(cells), value1(value1), value2(value2), value3(value3), range(range) {

}

void HiddenTripleHint::print_description(std::ostream &out) const {
    out << "hidden triple: cells:";
    for (std::vector<Cell *>::const_iterator i = cells.begin(); i
            != cells.end(); ++i)
        out << ' ' << print_row_col((*i)->get_idx());
    out << " values: (" << value1 << "," << value2 << "," << value3 << ") ";
    out << "range: " << range.get_name();
    out << " removing: " << print_choices_to_remove(get_choices_to_remove());
}

const char *HiddenTripleHint::get_name() const {
    return "Hidden triple";
}

void HiddenTripleHintProducer::fill_potential_values(const Range &range,
        Grid &grid, std::vector<int> &potentialvalues) const {
    std::set<int> v;

    for (Range::const_iterator i = range.begin(); i != range.end(); ++i) {
        const Cell &cell = grid[*i];
        if (!cell.has_value()) {
            for (int value = 1; value < 10; ++value) {
                if (cell.has_choice(value))
                    v.insert(value);
            }
        }
    }

    std::copy(v.begin(), v.end(), std::back_inserter(potentialvalues));
}

HiddenTripleHintProducer::HiddenTripleHintProducer() :
    degree(3) {

}

void HiddenTripleHintProducer::get_positions(const Range &range, Grid &grid,
        int value, std::bitset<9> &positions) const {
    for (int i = 0; i < 9; ++i) {
        int idx = range[i];
        positions[i] = grid[idx].has_choice(value);
    }
}

void HiddenTripleHintProducer::consume_possible_hint(const Range &range,
        Grid &grid, std::bitset<9> &positions, int value1, int value2,
        int value3, HintConsumer &consumer) const {
    std::vector<Cell *> cells;
    for (int i = 0; i < 9; ++i) {
        if (positions[i]) {
            int idx = range[i];
            Cell &cell = grid[idx];
            cells.push_back(&cell);
        }
    }
    bool ok = false;
    for (std::vector<Cell *>::const_iterator i = cells.begin(); i
            != cells.end(); ++i) {
        for (int value = 1; value < 10; ++value) {
            if (value != value1 && value != value2 && value != value3
                    && (*i)->has_choice(value)) {
                ok = true;
                break;
            }
        }
    }
    if (!ok)
        return;
    consumer.consume_hint(create_hint(cells, value1, value2, value3, range));
}

void HiddenTripleHintProducer::find_sets_for_range(const Range &range,
        Grid &grid, HintConsumer &consumer) const {
    std::vector<int> potenialvalues;
    fill_potential_values(range, grid, potenialvalues);
    if (potenialvalues.size() < degree) {
        return;
    }
    std::vector<std::bitset<9> > v(degree);

    int size = potenialvalues.size();
    for (int i = 0; i < size; ++i) {
        int value1 = potenialvalues[i];
        get_positions(range, grid, value1, v[0]);
        for (int j = i + 1; j < size; ++j) {
            int value2 = potenialvalues[j];
            get_positions(range, grid, value2, v[1]);
            for (int k = j + 1; k < size; ++k) {
                int value3 = potenialvalues[k];
                get_positions(range, grid, value3, v[2]);
                std::bitset<9> check;
                for (size_t l = 0; l < degree; ++l) {
                    check |= v[l];
                }
                if (check.count() == degree) {
                    consume_possible_hint(range, grid, check, value1, value2,
                            value3, consumer);
                    if (!consumer.wants_more_hints())
                        return;
                }
            }
        }
    }
}

void HiddenTripleHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    for (RangeList::const_iterator irange = RANGES.begin(); irange
            != RANGES.end(); ++irange) {
        find_sets_for_range(*irange, grid, consumer);
        if (!consumer.wants_more_hints())
            return;
    }
}

HiddenTripleHint *HiddenTripleHintProducer::create_hint(const std::vector<
        Cell *> &cells, int value1, int value2, int value3, const Range &range) const {
    HiddenTripleHint *hint = new HiddenTripleHint(cells, value1, value2,
            value3, range);

    for (std::vector<Cell *>::const_iterator i = cells.begin(); i
            != cells.end(); ++i) {
        for (int value = 1; value < 10; ++value) {
            if (value != value1 && value != value2 && value != value3) {
                Cell &cell = *(*i);
                if (cell.has_choice(value))
                    hint->add_choice_to_remove(&cell, value);
            }
        }
    }

    return hint;
}
