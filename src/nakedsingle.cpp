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
#include "nakedsingle.hpp"
#include "grid.hpp"
#include "hintconsumer.hpp"
#include "util.hpp"

NakedSingleHint::NakedSingleHint(int cell_idx, int value) :
    cell_idx(cell_idx), value(value) {
}

void NakedSingleHint::apply(Grid &grid) {
    Cell &cell = grid[cell_idx];
    cell.set_value(value);
    grid.cleanup_choice(cell);
}

void NakedSingleHint::print_description(std::ostream &out) const {
    out << "naked single: " << print_row_col(cell_idx) << "="
            << value;
}

const char *NakedSingleHint::get_name() const {
    return "Naked single";
}

void NakedSingleHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    Grid::iterator begin = grid.begin();
    Grid::iterator end = grid.end();
    for (Grid::iterator i = begin; i != end; ++i) {
        Cell &cell = *i;
        if (cell.get_num_choices() == 1) {
            if (!consumer.consume_hint(new NakedSingleHint(cell.get_idx(), cell.first_choice())))
                return;
        }
    }
}
