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

#ifndef BOXLINEREDUCTION_HPP_
#define BOXLINEREDUCTION_HPP_

#include "hint.hpp"
#include "range.hpp"

#include <vector>
#include <bitset>

class Cell;
class Grid;

class BoxLineReductionHint: public Hint {
    Grid &grid;
    int value;
    std::vector<int> cells_to_clear;
    const Range &block1;
    const Range &block2;
    const Range &block3;
    const Range &range1;
    const Range &range2;
    const Range &range3;

public:
    BoxLineReductionHint(Grid &grid, int value,
            const std::vector<int> &cells_to_clear, const Range &block1,
            const Range &block2, const Range &block3, const Range &range1,
            const Range &range2, const Range &range3);
    void apply();
    void print_description(std::ostream &out) const;
    const char *get_name() const;
};

class BoxLineReductionHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void find_line_hints(const std::vector<Range> &ranges, int value,
            int start, Grid &grid, HintConsumer &consumer) const;
    void get_row_indexes(const std::bitset<3> &ijrows, int rows[3]) const;
    void get_common_fields(const Range &range1, const Range &range2,
            std::vector<int> &fields) const;
    Hint *create_hint(Grid &grid, const std::vector<Range> &ranges, int value,
            int start, int i1, int i2, int i3, int rows[3]) const;
};

#endif
