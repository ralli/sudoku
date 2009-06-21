#include <iostream>

#include "xwing.hpp"
#include "range.hpp"
#include "grid.hpp"
#include "hintconsumer.hpp"

XWingRowHint::XWingRowHint(Grid &grid, int row1, int row2, int col1, int col2,
        int value) :
    grid(grid), row1(row1), row2(row2), col1(col1), col2(col2), value(value) {

}
void XWingRowHint::print_description(std::ostream &out) const {
    out << "x-wing (row): row1: " << row1 + 1 << " row2: " << row2 + 1
            << " column1: " << col1 + 1 << " column2: " << col2 + 1
            << " value: " << value << " removing: " << print_choices_to_remove(get_choices_to_remove());
}

XWingColumnHint::XWingColumnHint(Grid &grid, int row1, int row2, int col1,
        int col2, int value) :
    grid(grid), row1(row1), row2(row2), col1(col1), col2(col2), value(value) {

}

void XWingColumnHint::print_description(std::ostream &out) const {
    out << "x-wing (column): row1: " << row1 + 1 << " row2: " << row2 + 1
            << " column1: " << col1 + 1 << " column2: " << col2 + 1
            << " value: " << value << " removing: " << print_choices_to_remove(get_choices_to_remove());
}

void XWingHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    find_row_hints(grid, consumer);
    if (!consumer.wants_more_hints())
        return;
    find_col_hints(grid, consumer);
}

int XWingHintProducer::count_col_value(Grid &grid, int col, int value) const {
    const Range &range = RANGES.get_column(col);
    int count = 0;
    for (Range::const_iterator irange = range.begin(); irange != range.end(); ++irange) {
        Cell &cell = grid[*irange];
        if (cell.has_choice(value))
            ++count;
    }
    return count;
}

int XWingHintProducer::count_row_value(Grid &grid, int row, int value) const {
    const Range &range = RANGES.get_row(row);
    int count = 0;
    for (Range::const_iterator irange = range.begin(); irange != range.end(); ++irange) {
        Cell &cell = grid[*irange];
        if (cell.has_choice(value))
            ++count;
    }
    return count;
}

bool XWingHintProducer::check_second_row(int row, int col1, int col2,
        Grid & grid, int value) const {
    int idx1 = row * 9 + col1;
    int idx2 = row * 9 + col2;
    return grid[idx1].has_choice(value) && grid[idx2].has_choice(value)
            && count_row_value(grid, row, value) == 2 && (count_col_value(grid,
            col1, value) > 2 || count_col_value(grid, col2, value) > 2);
}

bool XWingHintProducer::check_second_col(int col, int row1, int row2,
        Grid & grid, int value) const {
    int idx1 = row1 * 9 + col;
    int idx2 = row2 * 9 + col;
    return grid[idx1].has_choice(value) && grid[idx2].has_choice(value)
            && count_col_value(grid, col, value) == 2 && (count_row_value(grid,
            row1, value) > 2 || count_row_value(grid, row2, value) > 2);
}

void XWingHintProducer::find_row_hints(Grid &grid, HintConsumer &consumer) const {
    for (int row = 0; row < 9; ++row) {
        std::vector<std::vector<Cell *> > freq(10);
        fill_row_freq(grid, row, freq);
        for (int value = 1; value < 10; ++value) {
            if (freq[value].size() == 2) {
                int col1 = freq[value][0]->get_col();
                int col2 = freq[value][1]->get_col();
                for (int row2 = row + 1; row2 < 9; ++row2) {
                    if (check_second_row(row2, col1, col2, grid, value)) {
                        if (!consumer.consume_hint(create_row_hint(grid, row,
                                row2, col1, col2, value)))
                            return;
                    }
                }
            }
        }
    }
}

void XWingHintProducer::fill_row_freq(Grid &grid, int row, std::vector<
        std::vector<Cell *> > &freq) const {
    const Range &range = RANGES.get_row(row);
    for (Range::const_iterator irange = range.begin(); irange != range.end(); ++irange) {
        Cell &cell = grid[*irange];
        for (int value = 1; value < 10; ++value) {
            if (cell.has_choice(value)) {
                freq[value].push_back(&cell);
            }
        }
    }
}

void XWingHintProducer::find_col_hints(Grid &grid, HintConsumer &consumer) const {
    for (int col = 0; col < 9; ++col) {
        std::vector<std::vector<Cell *> > freq(10);
        fill_col_freq(grid, col, freq);
        for (int value = 1; value < 10; ++value) {
            if (freq[value].size() == 2) {
                int row1 = freq[value][0]->get_row();
                int row2 = freq[value][1]->get_row();
                for (int col2 = col + 1; col2 < 9; ++col2) {
                    if (check_second_col(col2, row1, row2, grid, value)) {
                        if (!consumer.consume_hint(create_column_hint(grid,
                                row1, row2, col, col2, value)))
                            return;
                    }
                }
            }
        }
    }
}

void XWingHintProducer::fill_col_freq(Grid &grid, int col, std::vector<
        std::vector<Cell *> > &freq) const {
    const Range &range = RANGES.get_column(col);
    for (Range::const_iterator irange = range.begin(); irange != range.end(); ++irange) {
        Cell &cell = grid[*irange];
        for (int value = 1; value < 10; ++value) {
            if (cell.has_choice(value)) {
                freq[value].push_back(&cell);
            }
        }
    }
}

XWingRowHint *XWingHintProducer::create_row_hint(Grid &grid, int row1,
        int row2, int col1, int col2, int value) const {
    XWingRowHint *hint = new XWingRowHint(grid, row1, row2, col1, col2, value);

    const Range &range1 = RANGES.get_column(col1);
    const Range &range2 = RANGES.get_column(col2);

    for (int row = 0; row < 9; ++row) {
        if (row != row1 && row != row2) {
            if (grid[range1[row]].has_choice(value))
                hint->add_choice_to_remove(&grid[range1[row]], value);
            if (grid[range2[row]].has_choice(value))
                hint->add_choice_to_remove(&grid[range2[row]], value);
        }
    }

    return hint;
}

XWingColumnHint *XWingHintProducer::create_column_hint(Grid &grid, int row1,
        int row2, int col1, int col2, int value) const {
    XWingColumnHint* hint = new XWingColumnHint(grid, row1, row2, col1, col2,
            value);

    const Range &range1 = RANGES.get_row(row1);
    const Range &range2 = RANGES.get_row(row2);

    for (int col = 0; col < 9; ++col) {
        if (col != col1 && col != col2) {
            if (grid[range1[col]].has_choice(value))
                hint->add_choice_to_remove(&grid[range1[col]], value);
            if (grid[range2[col]].has_choice(value))
                hint->add_choice_to_remove(&grid[range2[col]], value);
        }
    }

    return hint;
}
