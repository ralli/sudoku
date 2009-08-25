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

#ifndef INDIRECTHINT_HPP_
#define INDIRECTHINT_HPP_

#include "hint.hpp"
#include <vector>
#include <utility>

class Cell;

class IndirectHint: public Hint {
    std::vector<std::pair<int, int> > choices_to_remove;
public:
    void add_choice_to_remove(Cell *cell, int value);
    const std::vector<std::pair<int, int> > &get_choices_to_remove() const;
    void apply(Grid &grid);
};


inline const std::vector<std::pair<int, int> > &IndirectHint::get_choices_to_remove() const {
    return choices_to_remove;
}

struct print_choices_to_remove {
    const std::vector<std::pair<int, int> > &choices_to_remove;

    print_choices_to_remove(
            const std::vector<std::pair<int, int> > &choices_to_remove) :
        choices_to_remove(choices_to_remove) {

    }

    void print(std::ostream &out) const;
};

std::ostream &operator <<(std::ostream &out, const print_choices_to_remove &x);

#endif /* INDIRECTHINT_HPP_ */
