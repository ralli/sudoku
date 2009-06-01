#include <iostream>
#include <bitset>

#include "nakeddouble.hpp"
#include "range.hpp"
#include "hintconsumer.hpp"
#include "grid.hpp"
#include "util.hpp"

NakedDoubleHint::NakedDoubleHint(Grid &grid, int first_cell_idx, int second_cell_idx, int value1, int value2, const Range &range) 
: grid(grid), first_cell_idx(first_cell_idx), second_cell_idx(second_cell_idx), value1(value1), value2(value2), range(range)
{
}

void NakedDoubleHint::apply() {
    for(int i = 0; i < 9; ++i) {
        if(range[i] != first_cell_idx && range[i] != second_cell_idx) {
            Cell &cell = grid[range[i]];
            cell.remove_choice(value1);
            cell.remove_choice(value2);
        }
    }
}

void NakedDoubleHint::print_description(std::ostream &out) const {
    out << "naked double: cell1: " << print_row_col(first_cell_idx) << " cell2: " << print_row_col(second_cell_idx) << " in range: " << range.get_name();
}

void fill_bitset(Cell &cell, std::bitset<10> &bits) {
    for(int value = 1; value < 10; ++value) {
        bits[value] = cell.has_choice(value);
    }
}

void fill_values(const Cell &cell, std::vector<int> &values)
{
    for(int value = 1; value < 10; ++value) {
        if(cell.has_choice(value))
            values.push_back(value);
    }
}

void NakedDoubleHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    std::vector<std::bitset<10> > bitsets(9);

    for(RangeList::const_iterator irange = RANGES.begin(); irange != RANGES.end(); ++irange) {
        for(int i = 0; i < 9; ++i) {
            int idx = (*irange)[i];
            Cell &cell = grid[idx];
            fill_bitset(cell, bitsets[i]);
        }


        for(int i = 0; i < 9; ++i) {
            if(bitsets[i].count() == 2) {
                int count=1;
                int to_eliminate=0;
                for(int j = i+1; j < 9; ++j) {
                    if(bitsets[i] == bitsets[j]) {
                        for(int k = 0; k < 9; ++k) {
                            if(k != i && k != j) {
                                if((bitsets[i] & bitsets[k]).any()) {
                                    int idx1 = (*irange)[i];
                                    int idx2 = (*irange)[j];
                                    std::vector<int> values;
                                    fill_values(grid[idx1], values);
                                    if(!consumer.consume_hint(new NakedDoubleHint(grid, idx1, idx2, values[0], values[1], *irange)))
                                        return;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

