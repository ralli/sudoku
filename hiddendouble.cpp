#include <iostream>

#include "hiddendouble.hpp"
#include "grid.hpp"
#include "range.hpp"
#include "hintconsumer.hpp"

HiddenDoubleHint::HiddenDoubleHint(std::vector<Cell *> cells, std::pair<int,
        int> pair, const Range &range) :
    cells(cells), pair(pair), range(range) {

}

void HiddenDoubleHint::apply() {
    for (std::vector<Cell *>::const_iterator i = cells.begin(); i
            != cells.end(); ++i) {
        (*i)->clear_choices();
        (*i)->add_choice(pair.first);
        (*i)->add_choice(pair.second);
    }
}

void HiddenDoubleHint::print_description(std::ostream &out) const {
    out << "hidden double: cells: ";
    for (std::vector<Cell *>::const_iterator i = cells.begin(); i
            != cells.end(); ++i) {
        out << "(" << (*i)->get_row() + 1 << "," << (*i)->get_col() + 1 << ") ";
    }
    out << " pair: (" << pair.first << "," << pair.second << ") range: "
            << range.get_name();
}

void HiddenDoubleHintProducer::build_frequencies(Grid &grid,
        const Range &range, std::vector<std::vector<Cell *> > &frequencies) {
    for (Range::const_iterator i = range.begin(); i != range.end(); ++i) {
        Cell &cell = grid[*i];
        if (cell.has_value())
            continue;
        for (int value = 1; value < 10; ++value) {
            if (cell.has_choice(value))
                frequencies[value].push_back(&cell);
        }
    }
}

void HiddenDoubleHintProducer::build_pair_map(Grid &grid, const Range &range,
        pair_map &pairs) {
    typedef std::vector<std::vector<Cell *> > frequency_vector;
    frequency_vector frequencies(10);
    build_frequencies(grid, range, frequencies);
    for (int value = 1; value < 10; ++value) {
        if (frequencies[value].size() == 2) {
            for (std::vector<Cell *>::iterator icell =
                    frequencies[value].begin(); icell
                    != frequencies[value].end(); ++icell) {
                if ((*icell)->has_choice(value)) {
                    for (int second_value = value + 1; second_value < 10; ++second_value) {
                        if (frequencies[second_value].size() == 2
                                && (*icell)->has_choice(second_value)) {
                            pairs[std::pair<int, int>(value, second_value)].push_back(
                                    *icell);
                        }
                    }
                }
            }
        }
    }
}

void HiddenDoubleHintProducer::find_hints(Grid & grid, HintConsumer & consumer) {
    for (RangeList::const_iterator irange = RANGES.begin(); irange
            != RANGES.end(); ++irange) {
        std::map<std::pair<int, int>, std::vector<Cell *> > pairs;
        build_pair_map(grid, *irange, pairs);
        for (pair_map::iterator ipair = pairs.begin(); ipair != pairs.end(); ++ipair) {
            std::vector<Cell *> cells = ipair->second;
            if (cells.size() == 2) {
                for (std::vector<Cell *>::iterator i = cells.begin(); i
                        != cells.end(); ++i) {
                    if ((*i)->get_num_choices() > 2) {
                        if(!consumer.consume_hint(new HiddenDoubleHint(cells,
                                ipair->first, *irange))) {
                            return;
                        }
                        break;
                    }
                }
            }
        }
    }
}
