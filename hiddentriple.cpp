#include <iostream>
#include <vector>
#include <set>
#include <bitset>

#include "hiddentriple.hpp"
#include "hintconsumer.hpp"
#include "range.hpp"
#include "util.hpp"
#include "grid.hpp"

HiddenTripleHint::HiddenTripleHint(std::vector<Cell *> &cells, int value1,
        int value2, int value3, const Range &range) :
    cells(cells), value1(value1), value2(value2), value3(value3), range(range) {

}
void HiddenTripleHint::apply() {
    for (std::vector<Cell *>::iterator i = cells.begin(); i != cells.end(); ++i) {
        for (int value = 1; value < 10; ++value) {
            if (value != value1 && value != value2 && value != value3)
                (*i)->remove_choice(value);
        }
    }
}

void HiddenTripleHint::print_description(std::ostream &out) const {
    out << "hidden triple: cells: ";
    for (std::vector<Cell *>::const_iterator i = cells.begin(); i
            != cells.end(); ++i)
        out << print_row_col((*i)->get_idx());
    out << " values: (" << value1 << "," << value2 << "," << value3 << ") ";
    out << "range: " << range.get_name();
}

void HiddenTripleHintProducer::fill_potential_values(const Range &range,
        Grid &grid, std::vector<int> &potentialvalues) const {
    std::set<int> v;

    for (Range::const_iterator i = range.begin(); i != range.end(); ++i) {
        const Cell &cell = grid[*i];
        if (!cell.has_value()) {
            for (int value = 1; value < 10; ++value) {
                if (cell.has_choice(value))
                    v.insert(value);
            }
        }
    }

    std::copy(v.begin(), v.end(), std::back_inserter(potentialvalues));
}

HiddenTripleHintProducer::HiddenTripleHintProducer() :
    degree(3) {

}

void HiddenTripleHintProducer::get_positions(const Range &range, Grid &grid,
        int value, std::bitset<9> &positions) const {
    for (int i = 0; i < 9; ++i) {
        int idx = range[i];
        positions[i] = grid[idx].has_choice(value);
    }
}

void HiddenTripleHintProducer::consume_possible_hint(const Range &range,
        Grid &grid, std::bitset<9> &positions, int value1, int value2,
        int value3, HintConsumer &consumer) const {
    std::vector<Cell *> cells;
    for (int i = 0; i < 9; ++i) {
        if (positions[i]) {
            int idx = range[i];
            Cell &cell = grid[idx];
            cells.push_back(&cell);
        }
    }
    bool ok = false;
    for (std::vector<Cell *>::const_iterator i = cells.begin(); i
            != cells.end(); ++i) {
        for (int value = 1; value < 10; ++value) {
            if (value != value1 && value != value2 && value != value3
                    && (*i)->has_choice(value)) {
                ok = true;
                break;
            }
        }
    }
    if (!ok)
        return;
    consumer.consume_hint(new HiddenTripleHint(cells, value1, value2, value3,
            range));
}

void HiddenTripleHintProducer::find_sets_for_range(const Range &range,
        Grid &grid, HintConsumer &consumer) const {
    std::vector<int> potenialvalues;
    fill_potential_values(range, grid, potenialvalues);
    if (potenialvalues.size() < degree) {
        return;
    }
    std::vector<std::bitset<9> > v(degree);

    int size = potenialvalues.size();
    for (int i = 0; i < size; ++i) {
        int value1 = potenialvalues[i];
        get_positions(range, grid, value1, v[0]);
        for (int j = i + 1; j < size; ++j) {
            int value2 = potenialvalues[j];
            get_positions(range, grid, value2, v[1]);
            for (int k = j + 1; k < size; ++k) {
                int value3 = potenialvalues[k];
                get_positions(range, grid, value3, v[2]);
                std::bitset<9> check;
                for (size_t l = 0; l < degree; ++l) {
                    check |= v[l];
                }
                if (check.count() == degree) {
                    consume_possible_hint(range, grid, check, value1, value2,
                            value3, consumer);
                    if (!consumer.wants_more_hints())
                        return;
                }
            }
        }
    }
}

void HiddenTripleHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    for (RangeList::const_iterator irange = RANGES.begin(); irange
            != RANGES.end(); ++irange) {
        find_sets_for_range(*irange, grid, consumer);
        if (!consumer.wants_more_hints())
            return;
    }
}
