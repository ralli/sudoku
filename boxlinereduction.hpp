#ifndef BOXLINEREDUCTION_HPP_
#define BOXLINEREDUCTION_HPP_

#include "hint.hpp"
#include "range.hpp"

#include <vector>
#include <bitset>

class Cell;
class Grid;

class BoxLineReductionHint: public Hint {
    Grid &grid;
    int value;
    std::vector<int> cells_to_clear;
    const Range &block1;
    const Range &block2;
    const Range &block3;
    const Range &range1;
    const Range &range2;
    const Range &range3;

public:
    BoxLineReductionHint(Grid &grid, int value,
            const std::vector<int> &cells_to_clear, const Range &block1,
            const Range &block2, const Range &block3, const Range &range1,
            const Range &range2, const Range &range3);
    void apply();
    void print_description(std::ostream &out) const;
};

class BoxLineReductionHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void find_line_hints(const std::vector<Range> &ranges, int value,
            int start, Grid &grid, HintConsumer &consumer) const;
    void get_row_indexes(const std::bitset<3> &ijrows, int rows[3]) const;
    void get_common_fields(const Range &range1, const Range &range2,
            std::vector<int> &fields) const;
    Hint *create_hint(Grid &grid, const std::vector<Range> &ranges, int value,
            int start, int i1, int i2, int i3, int rows[3]) const;
};

#endif
