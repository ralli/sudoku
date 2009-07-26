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

#ifndef XYWING_HPP
#define XYWING_HPP

#include <vector>

#include "indirecthint.hpp"

class Grid;
class Cell;

class XYWingHint: public IndirectHint {
    const Cell &xy;
    const Cell &yz;
    const Cell &xz;
    int x;
    int y;
    int z;
public:
    XYWingHint(const Cell &xy, const Cell &yz, const Cell &xz, int x, int y,
            int z);
    void print_description(std::ostream &out) const;
    const char *get_name() const;
};

class XYWingHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void find_xy_wing(Cell &cell, Grid &grid, HintConsumer &consumer) const;
    void fill_cell_values(const Cell &cell, int values[2]) const;
    XYWingHint *create_xy_hint(Cell &xy, Cell &yz, Cell &xz, int x, int y,
            int z, Grid &grid) const;
    bool shares_region(const Cell &a, const Cell &b) const;
    XYWingHint *create_hint(const Cell &xy, const Cell &yz, const Cell &xz, int x, int y,
            int z, Grid &grid, const std::vector<int> &cells_to_clear) const;
};

#endif
