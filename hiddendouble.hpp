#ifndef HIDDENDOUBLE_HPP_
#define HIDDENDOUBLE_HPP_

#include <vector>
#include <map>

#include "indirecthint.hpp"

class Cell;
class Range;
class Grid;
class HintConsumer;

class HiddenDoubleHint: public IndirectHint {
    std::vector<Cell *> cells;
    std::pair<int, int> pair;
    const Range &range;
public:
    HiddenDoubleHint(std::vector<Cell *> cells, std::pair<int, int> pair,
            const Range &range);
    virtual void print_description(std::ostream &out) const;
};

class HiddenDoubleHintProducer: public HintProducer {
    typedef std::map<std::pair<int, int>, std::vector<Cell *> > pair_map;
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void build_pair_map(Grid &grid, const Range &range, pair_map &pairs);
    void build_frequencies(Grid &grid, const Range &range, std::vector<
            std::vector<Cell *> > &frequencies);
    HiddenDoubleHint *create_hint(const std::vector<Cell *> &cells,
            const std::pair<int, int> &values, const Range &range) const;
};

#endif
