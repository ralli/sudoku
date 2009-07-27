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

#include "indirecthint.hpp"
#include "grid.hpp"
#include "util.hpp"

void IndirectHint::add_choice_to_remove(Cell *cell, int value) {
    choices_to_remove.push_back(std::pair<int, int>(cell->get_idx(), value));
}

void IndirectHint::apply(Grid &grid) {
    for (std::vector<std::pair<int, int> >::iterator i =
            choices_to_remove.begin(); i != choices_to_remove.end(); ++i) {
        Cell &cell = grid[i->first];
        cell.remove_choice(i->second);
    }
}


void print_choices_to_remove::print(std::ostream &out) const {
    for (std::vector<std::pair<int, int> >::const_iterator i =
            choices_to_remove.begin(); i != choices_to_remove.end(); ++i) {
        out << print_row_col(i->first) << "=" << i->second;
        if ((i + 1) != choices_to_remove.end())
            out << ' ';
    }
}

std::ostream &operator <<(std::ostream &out, const print_choices_to_remove &x) {
    x.print(out);
    return out;
}

