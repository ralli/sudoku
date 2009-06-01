#ifndef NAKED_DOUBLE_HPP
#define NAKED_DOUBLE_HPP

#include "hint.hpp"

class Range;

class NakedDoubleHint : public Hint {
    Grid &grid;
    int first_cell_idx;
    int second_cell_idx;
    int value1;
    int value2;
    const Range &range;
public:
    NakedDoubleHint(Grid &grid, int first_cell_idx, int second_cell_idx, int value1, int value2, const Range &range);
    void apply();
    void print_description(std::ostream &out) const;
};

class NakedDoubleHintProducer : public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
};

#endif
