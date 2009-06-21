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

#include "range.hpp"
#include <sstream>
#include <set>

const RangeList RANGES;

RangeList::RangeList() {
    int indexes[9];

    for (int row = 0; row < 9; ++row) {
        std::ostringstream os;
        os << "row " << row + 1;
        for (int col = 0; col < 9; ++col) {
            indexes[col] = row * 9 + col;
        }
        ranges.push_back(Range(os.str(), indexes));
        rows.push_back(Range(os.str(), indexes));
    }

    for (int col = 0; col < 9; ++col) {
        std::ostringstream os;
        os << "col " << col + 1;
        for (int row = 0; row < 9; ++row) {
            indexes[row] = row * 9 + col;
        }
        ranges.push_back(Range(os.str(), indexes));
        columns.push_back(Range(os.str(), indexes));
    }

    for (int i = 0; i < 3; ++i) {
        int start_row = 3 * i;

        for (int j = 0; j < 3; ++j) {
            int start_col = 3 * j;
            std::ostringstream os;
            os << "block(" << i + 1 << ',' << j + 1 << ')';

            int idx = 0;
            for (int row = start_row; row < start_row + 3; ++row) {
                for (int col = start_col; col < start_col + 3; ++col) {
                    indexes[idx] = row * 9 + col;
                    ++idx;
                }
            }

            ranges.push_back(Range(os.str(), indexes));
            blocks.push_back(Range(os.str(), indexes));
        }
    }

    field_ranges.resize(81);
    for (const_iterator i = begin(); i != end(); ++i) {
        for (Range::const_iterator j = i->begin(); j != i->end(); ++j) {
            field_ranges[*j].push_back(*i);
        }
    }

    field_neighbours.resize(81);
    for (int idx = 0; idx < 81; ++idx) {
        const_iterator begin = field_ranges[idx].begin();
        const_iterator end = field_ranges[idx].end();
        std::set<int> neighbours;
        for (const_iterator irange = begin; irange != end; ++irange) {
            for (Range::const_iterator j = irange->begin(); j != irange->end(); ++j) {
                if (*j != idx) {
                    neighbours.insert(*j);
                }
            }
        }
        std::copy(neighbours.begin(), neighbours.end(), std::back_inserter(
                field_neighbours[idx]));
    }
}

RangeList::RangeList(const RangeList &other) :
    ranges(other.ranges), field_neighbours(other.field_neighbours), rows(other.rows),
            columns(other.columns), blocks(other.blocks) {
}

RangeList &RangeList::operator =(const RangeList &other) {
    if (this == &other)
        return *this;

    ranges = other.ranges;
    rows = other.rows;
    columns = other.columns;
    blocks = other.blocks;
    field_neighbours = other.field_neighbours;

    return *this;
}

