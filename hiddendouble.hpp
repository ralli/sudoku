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

#ifndef HIDDENDOUBLE_HPP_
#define HIDDENDOUBLE_HPP_

#include <vector>
#include <map>

#include "indirecthint.hpp"

class Cell;
class Range;
class Grid;
class HintConsumer;

class HiddenDoubleHint: public IndirectHint {
    std::vector<Cell *> cells;
    std::pair<int, int> pair;
    const Range &range;
public:
    HiddenDoubleHint(std::vector<Cell *> cells, std::pair<int, int> pair,
            const Range &range);
    virtual void print_description(std::ostream &out) const;
};

class HiddenDoubleHintProducer: public HintProducer {
    typedef std::map<std::pair<int, int>, std::vector<Cell *> > pair_map;
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void build_pair_map(Grid &grid, const Range &range, pair_map &pairs);
    void build_frequencies(Grid &grid, const Range &range, std::vector<
            std::vector<Cell *> > &frequencies);
    HiddenDoubleHint *create_hint(const std::vector<Cell *> &cells,
            const std::pair<int, int> &values, const Range &range) const;
};

#endif
