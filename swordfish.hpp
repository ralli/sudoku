#ifndef SWORDFISH_HPP
#define SWORDFISH_HPP

#include <vector>

#include "hint.hpp"
#include "range.hpp"

class Grid;

class SwordfishHint: public Hint {
    Grid &grid;
    int value;
    std::vector<const Range *> row_ranges;
    std::vector<const Range *> col_ranges;
    std::vector<int> cells_to_clear;
public:
    SwordfishHint(Grid &grid,
            int value,
            const std::vector<const Range *> &row_ranges,
            const std::vector<const Range *> &col_ranges,
            const std::vector<int> &cells_to_clear);
    void apply();
    void print_description(std::ostream &out) const;
};

class SwordfishHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void find_swordfishes(int value, Grid &grid,
            const std::vector<Range> &row_ranges,
            const std::vector<Range> &col_ranges, HintConsumer &consumer) const;
    SwordfishHint *create_swordfish_hint(int value, int row1, int row2,
            int row3, const std::bitset<9> &columnset,
            const std::vector<Range> &ranges,
            const std::vector<Range> &secondary_ranges, Grid &grid) const;
    void fill_rowsets(int value, Grid &grid, const std::vector<Range> &ranges,
            std::vector<std::bitset<9> > &bitsets) const;
};

#endif
