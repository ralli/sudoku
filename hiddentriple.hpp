#ifndef HIDDENTRIPLE_HPP_
#define HIDDENTRIPLE_HPP_

#include <vector>
#include <bitset>
#include "hint.hpp"

class Cell;
class Grid;
class Range;

class HiddenTripleHint: public Hint {
    std::vector<Cell *> &cells;
    int value1;
    int value2;
    int value3;
    const Range &range;
public:
    HiddenTripleHint(std::vector<Cell *> &cells, int value1, int value2,
            int value3, const Range &range);
    void apply();
    void print_description(std::ostream &out) const;
};

class HiddenTripleHintProducer: public HintProducer {
    size_t degree;
public:
    HiddenTripleHintProducer();
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void fill_potential_values(const Range &range, Grid &grid,
            std::vector<int> &potentialvalues) const;
    void get_positions(const Range &range, Grid &grid, int value,
            std::bitset<9> &positions) const;

    void consume_possible_hint(const Range &range, Grid &grid,
            std::bitset<9> &positions, int value1, int value2, int value3,
            HintConsumer &consumer) const;
    void find_sets_for_range(const Range &range, Grid &grid,
            HintConsumer &consumer) const;
};
#endif
