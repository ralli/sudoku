#include <iostream>
#include <vector>
#include <bitset>

#include "swordfish.hpp"
#include "util.hpp"
#include "range.hpp"
#include "grid.hpp"
#include "hintconsumer.hpp"

SwordfishHint::SwordfishHint(int value,
        const std::vector<const Range *> &row_ranges, const std::vector<
                const Range *> &col_ranges) :
    value(value), row_ranges(row_ranges), col_ranges(col_ranges) {

}

void SwordfishHint::print_description(std::ostream &out) const {
    out << "swordfish: value: " << value << " first dimension: ";
    for (std::vector<const Range *>::const_iterator i = row_ranges.begin(); i
            != row_ranges.end(); ++i)
        out << (*i)->get_name() << ' ';
    out << "second dimension: ";
    for (std::vector<const Range *>::const_iterator i = col_ranges.begin(); i
            != col_ranges.end(); ++i)
        out << (*i)->get_name() << ' ';
    out << " removing: " << print_choices_to_remove(get_choices_to_remove());
}

void SwordfishHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    for (int value = 1; value < 10; ++value) {
        find_swordfishes(value, grid, RANGES.get_rows(), RANGES.get_columns(),
                consumer);
        if (!consumer.wants_more_hints())
            return;
        find_swordfishes(value, grid, RANGES.get_columns(), RANGES.get_rows(),
                consumer);
        if (!consumer.wants_more_hints())
            return;
    }
}

void SwordfishHintProducer::find_swordfishes(int value, Grid &grid,
        const std::vector<Range> &row_ranges,
        const std::vector<Range> &col_ranges, HintConsumer &consumer) const {
    std::vector<std::bitset<9> > rowsets(9);

    fill_rowsets(value, grid, row_ranges, rowsets);

    for (int i = 0; i < 9; ++i) {
        if (rowsets[i].count() > 1) {
            for (int j = i + 1; j < 9; ++j) {
                if (rowsets[j].count() > 1) {
                    for (int k = 0; k < 9; ++k) {
                        if (k != i && k != j && rowsets[k].count() > 1) {
                            std::bitset<9> x = rowsets[i];
                            x |= rowsets[j];
                            x |= rowsets[k];
                            if (x.count() == 3) {
                                SwordfishHint *hint = create_swordfish_hint(
                                        value, i, j, k, x, row_ranges,
                                        col_ranges, grid);
                                if (hint) {
                                    if (!consumer.consume_hint(hint))
                                        return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

SwordfishHint *SwordfishHintProducer::create_swordfish_hint(int value,
        int row1, int row2, int row3, const std::bitset<9> &columnset,
        const std::vector<Range> &ranges,
        const std::vector<Range> &secondary_ranges, Grid &grid) const {
    std::vector<int> cols;
    std::vector<int> cells_to_clear;

    for (int i = 0; i < 9; ++i) {
        if (columnset[i]) {
            cols.push_back(i);
        }
    }

    for (int row = 0; row < 9; ++row) {
        if (row != row1 && row != row2 && row != row3) {
            for (int colidx = 0; colidx < 3; ++colidx) {
                int col = cols[colidx];
                if (grid[ranges[row][col]].has_choice(value)) {
                    cells_to_clear.push_back(ranges[row][col]);
                }
            }
        }
    }

    if (cells_to_clear.empty())
        return 0;

    std::vector<const Range *> row_ranges;
    row_ranges.push_back(&ranges[row1]);
    row_ranges.push_back(&ranges[row2]);
    row_ranges.push_back(&ranges[row3]);
    std::vector<const Range *> col_ranges;
    col_ranges.push_back(&secondary_ranges[cols[0]]);
    col_ranges.push_back(&secondary_ranges[cols[1]]);
    col_ranges.push_back(&secondary_ranges[cols[2]]);

    return create_hint(grid, value, row_ranges, col_ranges, cells_to_clear);
}

void SwordfishHintProducer::fill_rowsets(int value, Grid &grid,
        const std::vector<Range> &ranges, std::vector<std::bitset<9> > &bitsets) const {
    for (int i = 0; i < 9; ++i) {
        const Range &range = ranges[i];
        for (int j = 0; j < 9; ++j) {
            bitsets[i][j] = grid[range[j]].has_choice(value);
        }
    }
}

SwordfishHint *SwordfishHintProducer::create_hint(Grid &grid, int value,
        const std::vector<const Range *> &row_ranges, const std::vector<
                const Range *> &col_ranges,
        const std::vector<int> &cells_to_clear) const {
    SwordfishHint *hint = new SwordfishHint(value, row_ranges, col_ranges);
    for (std::vector<int>::const_iterator i = cells_to_clear.begin(); i
            != cells_to_clear.end(); ++i) {
        hint->add_choice_to_remove(&grid[(*i)], value);
    }
    return hint;
}
