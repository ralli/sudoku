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

#ifndef SWORDFISH_HPP
#define SWORDFISH_HPP

#include <vector>

#include "indirecthint.hpp"
#include "range.hpp"

class Grid;

class SwordfishHint: public IndirectHint {
    int value;
    std::vector<const Range *> row_ranges;
    std::vector<const Range *> col_ranges;
public:
    SwordfishHint(int value,
            const std::vector<const Range *> &row_ranges,
            const std::vector<const Range *> &col_ranges);
    void print_description(std::ostream &out) const;
};

class SwordfishHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void find_swordfishes(int value, Grid &grid,
            const std::vector<Range> &row_ranges,
            const std::vector<Range> &col_ranges, HintConsumer &consumer) const;
    SwordfishHint *create_swordfish_hint(int value, int row1, int row2,
            int row3, const std::bitset<9> &columnset,
            const std::vector<Range> &ranges,
            const std::vector<Range> &secondary_ranges, Grid &grid) const;
    void fill_rowsets(int value, Grid &grid, const std::vector<Range> &ranges,
            std::vector<std::bitset<9> > &bitsets) const;
    SwordfishHint *create_hint(Grid &grid,
            int value,
            const std::vector<const Range *> &row_ranges,
            const std::vector<const Range *> &col_ranges,
            const std::vector<int> &cells_to_clear) const;
};

#endif
