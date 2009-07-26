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

#include "singlehint.hpp"
#include "grid.hpp"
#include "range.hpp"
#include "hintconsumer.hpp"
#include "util.hpp"

#include <iostream>

SingleHint::SingleHint(Grid &grid, Cell &cell, int value, const Range &range) :
    grid(grid), cell(cell), value(value), range(range) {
}

void SingleHint::apply() {
    cell.set_value(value);
    grid.cleanup_choice(cell);
}

void SingleHint::print_description(std::ostream &out) const {
  out << "single: cell: " 
      << print_row_col(cell.get_idx()) 
      << " value: " << value 
      << " range: " << range.get_name();
}

const char *SingleHint::get_name() const {
    return "Hidden single";
}

void SingleHintProducer::find_hints(Grid & grid, HintConsumer & consumer) {

    for (RangeList::const_iterator irange = RANGES.begin(); irange
            != RANGES.end(); ++irange) {
        std::vector<std::vector<Cell *> > frequencies(10);
        for (Range::const_iterator i = irange->begin(); i != irange->end(); ++i) {
            Cell &cell = grid[*i];
            if (cell.has_value())
                continue;
            for (int value = 1; value < 10; ++value) {
                if (cell.has_choice(value)) {
                    frequencies[value].push_back(&cell);
                }
            }
        }

        for (int value = 1; value < 10; ++value) {
            if (frequencies[value].size() == 1) {
                if (!consumer.consume_hint(new SingleHint(grid,
                        *frequencies[value][0], value, *irange))) {
                    return;
                }
            }
        }
    }
}
