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

#ifndef HIDDENTRIPLE_HPP_
#define HIDDENTRIPLE_HPP_

#include <vector>
#include <bitset>
#include "indirecthint.hpp"

class Cell;
class Grid;
class Range;

class HiddenTripleHint: public IndirectHint {
    std::vector<Cell *> cells;
    int value1;
    int value2;
    int value3;
    const Range &range;
public:
    HiddenTripleHint(const std::vector<Cell *> &cells, int value1, int value2,
            int value3, const Range &range);
    void print_description(std::ostream &out) const;
};

class HiddenTripleHintProducer: public HintProducer {
    size_t degree;
public:
    HiddenTripleHintProducer();
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void fill_potential_values(const Range &range, Grid &grid,
            std::vector<int> &potentialvalues) const;
    void get_positions(const Range &range, Grid &grid, int value,
            std::bitset<9> &positions) const;

    void consume_possible_hint(const Range &range, Grid &grid,
            std::bitset<9> &positions, int value1, int value2, int value3,
            HintConsumer &consumer) const;
    void find_sets_for_range(const Range &range, Grid &grid,
            HintConsumer &consumer) const;
    HiddenTripleHint *create_hint(const std::vector<Cell *> &cells, int value1, int value2,
            int value3, const Range &range) const;
};
#endif
