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

#include "hiddendouble.hpp"
#include "grid.hpp"
#include "range.hpp"
#include "hintconsumer.hpp"
#include "util.hpp"

HiddenDoubleHint::HiddenDoubleHint(std::vector<Cell *> cells, std::pair<int,
        int> pair, const Range &range) :
    cells(cells), pair(pair), range(range) {

}

void HiddenDoubleHint::print_description(std::ostream &out) const {
    out << "hidden double: cells: ";
    for (std::vector<Cell *>::const_iterator i = cells.begin(); i
            != cells.end(); ++i) {
        out << print_row_col((*i)->get_idx()) << ' ';
    }
    out << "pair: (" << pair.first << "," << pair.second << ") range: "
            << range.get_name();

    out << " removing: " << print_choices_to_remove(get_choices_to_remove());
}

void HiddenDoubleHintProducer::build_frequencies(Grid &grid,
        const Range &range, std::vector<std::vector<Cell *> > &frequencies) {
    for (Range::const_iterator i = range.begin(); i != range.end(); ++i) {
        Cell &cell = grid[*i];
        if (cell.has_value())
            continue;
        for (int value = 1; value < 10; ++value) {
            if (cell.has_choice(value))
                frequencies[value].push_back(&cell);
        }
    }
}

void HiddenDoubleHintProducer::build_pair_map(Grid &grid, const Range &range,
        pair_map &pairs) {
    typedef std::vector<std::vector<Cell *> > frequency_vector;
    frequency_vector frequencies(10);
    build_frequencies(grid, range, frequencies);
    for (int value = 1; value < 10; ++value) {
        if (frequencies[value].size() == 2) {
            for (std::vector<Cell *>::iterator icell =
                    frequencies[value].begin(); icell
                    != frequencies[value].end(); ++icell) {
                if ((*icell)->has_choice(value)) {
                    for (int second_value = value + 1; second_value < 10; ++second_value) {
                        if (frequencies[second_value].size() == 2
                                && (*icell)->has_choice(second_value)) {
                            pairs[std::pair<int, int>(value, second_value)].push_back(
                                    *icell);
                        }
                    }
                }
            }
        }
    }
}

void HiddenDoubleHintProducer::find_hints(Grid & grid, HintConsumer & consumer) {
    for (RangeList::const_iterator irange = RANGES.begin(); irange
            != RANGES.end(); ++irange) {
        std::map<std::pair<int, int>, std::vector<Cell *> > pairs;
        build_pair_map(grid, *irange, pairs);
        for (pair_map::iterator ipair = pairs.begin(); ipair != pairs.end(); ++ipair) {
            std::vector<Cell *> cells = ipair->second;
            if (cells.size() == 2) {
                for (std::vector<Cell *>::iterator i = cells.begin(); i
                        != cells.end(); ++i) {
                    if ((*i)->get_num_choices() > 2) {
                        if (!consumer.consume_hint(create_hint(cells,
                                ipair->first, *irange))) {
                            return;
                        }
                        break;
                    }
                }
            }
        }
    }
}

HiddenDoubleHint *HiddenDoubleHintProducer::create_hint(const std::vector<Cell *> &cells,
        const std::pair<int, int> &values, const Range &range) const {
    HiddenDoubleHint *hint = new HiddenDoubleHint(cells, values, range);
    for (std::vector<Cell *>::const_iterator i = cells.begin(); i
            != cells.end(); ++i) {
        for (int value = 1; value <= 9; ++value) {
            if (value != values.first && value != values.second
                    && (*i)->has_choice(value))
                hint->add_choice_to_remove(*i, value);
        }
    }
    return hint;
}
