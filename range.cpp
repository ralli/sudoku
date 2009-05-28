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

    std::vector<std::vector<Range> > field_ranges(81);
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

