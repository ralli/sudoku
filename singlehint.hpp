#ifndef SINGLEHINT_HPP_
#define SINGLEHINT_HPP_

#include "hint.hpp"

class Grid;
class Cell;
class Range;

class SingleHint: public Hint {
    Grid &grid;
    Cell &cell;
    int value;
    const Range &range;
public:
    SingleHint(Grid &grid, Cell &cell, int value, const Range &range);

    virtual void apply();
    virtual void print_description(std::ostream &out) const;
};

class SingleHintProducer: public HintProducer {
    void find_hints(Grid &grid, HintConsumer &consumer);
};

#endif /* SINGLEHINT_HPP_ */
