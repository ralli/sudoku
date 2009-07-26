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

#ifndef CLAIMING_HPP
#define CLAIMING_HPP

#include "indirecthint.hpp"

#include <vector>

class Cell;
class ClaimingRowHint : public IndirectHint {
    std::vector<Cell *> cells;
    int                 row;
    int                 block_idx;
    int                 value;
    Grid                &grid;
public:
    ClaimingRowHint(std::vector<Cell *> &cells,int row, int block_idx, int value, Grid &grid);
    void print_description(std::ostream &out) const;
    const char *get_name() const;
};

class ClaimingColumnHint : public IndirectHint {
    std::vector<Cell *> cells;
    int                 col;
    int                 block_idx;
    int                 value;
    Grid                &grid;
public:
    ClaimingColumnHint(std::vector<Cell *> &cells,int row, int block_idx, int value, Grid &grid);
    void print_description(std::ostream &out) const;
    const char *get_name() const;
};

class ClaimingHintProducer : public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    int find_block_by(int start_row, int start_col) const;
    void find_row_hint(int row, Grid &grid, HintConsumer &consumer) const;
    void find_col_hint(int col, Grid &grid, HintConsumer &consumer) const;
    bool check_block(const std::vector<Cell *> &cells, int block_idx, int value, Grid &grid) const;
    ClaimingRowHint *create_row_hint(std::vector<Cell *> &cells,int row, int block_idx, int value, Grid &grid) const;
    ClaimingColumnHint *create_column_hint(std::vector<Cell *> &cells,int row, int block_idx, int value, Grid &grid) const;
};

#endif
