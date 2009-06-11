#include "singlehint.hpp"
#include "grid.hpp"
#include "range.hpp"
#include "hintconsumer.hpp"
#include "util.hpp"

#include <iostream>

SingleHint::SingleHint(Grid &grid, Cell &cell, int value, const Range &range) :
    grid(grid), cell(cell), value(value), range(range) {
}

void SingleHint::apply() {
    cell.set_value(value);
    grid.cleanup_choice(cell);
}

void SingleHint::print_description(std::ostream &out) const {
  out << "single: cell: " 
      << print_row_col(cell.get_idx()) 
      << " value: " << value 
      << " range: " << range.get_name();
}

void SingleHintProducer::find_hints(Grid & grid, HintConsumer & consumer) {

    for (RangeList::const_iterator irange = RANGES.begin(); irange
            != RANGES.end(); ++irange) {
        std::vector<std::vector<Cell *> > frequencies(10);
        for (Range::const_iterator i = irange->begin(); i != irange->end(); ++i) {
            Cell &cell = grid[*i];
            if (cell.has_value())
                continue;
            for (int value = 1; value < 10; ++value) {
                if (cell.has_choice(value)) {
                    frequencies[value].push_back(&cell);
                }
            }
        }

        for (int value = 1; value < 10; ++value) {
            if (frequencies[value].size() == 1) {
                if (!consumer.consume_hint(new SingleHint(grid,
                        *frequencies[value][0], value, *irange))) {
                    return;
                }
            }
        }
    }
}
