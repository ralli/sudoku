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

#ifndef XWING_HPP_
#define XWING_HPP_

#include <vector>
#include "indirecthint.hpp"

class Cell;

class XWingRowHint: public IndirectHint {
    Grid &grid;
    int row1;
    int row2;
    int col1;
    int col2;
    int value;
public:
    XWingRowHint(Grid &grid, int row1, int row2, int col1, int col2, int value);
    void print_description(std::ostream &out) const;
};

class XWingColumnHint: public IndirectHint {
    Grid &grid;
    int row1;
    int row2;
    int col1;
    int col2;
    int value;
public:
    XWingColumnHint(Grid &grid, int row1, int row2, int col1, int col2,
            int value);
    void print_description(std::ostream &out) const;
};

class XWingHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    int count_col_value(Grid &grid, int col, int value) const;
    int count_row_value(Grid &grid, int row, int value) const;
    bool
            check_second_row(int row2, int col1, int col2, Grid & grid,
                    int value) const;
    bool
            check_second_col(int col, int row1, int row2, Grid & grid,
                    int value) const;
    void find_row_hints(Grid &grid, HintConsumer &consumer) const;
    void find_col_hints(Grid &grid, HintConsumer &consumer) const;
    void fill_row_freq(Grid &grid, int row,
            std::vector<std::vector<Cell *> > &freq) const;
    void fill_col_freq(Grid &grid, int col,
            std::vector<std::vector<Cell *> > &freq) const;
    XWingRowHint *create_row_hint(Grid &grid, int row1, int row2, int col1,
            int col2, int value) const;
    XWingColumnHint *create_column_hint(Grid &grid, int row1, int row2,
            int col1, int col2, int value) const;

};

#endif
