#ifndef POINTING_HPP_
#define POINTING_HPP_

#include "indirecthint.hpp"

class PointingHint: public IndirectHint {
    Grid &grid;
    int start_row;
    int start_col;
    int value;
    int row;
    int col;
public:
    PointingHint(Grid &grid, int start_row, int start_col, int value, int row,
            int col);
    void print_description(std::ostream &out) const;
    bool is_row_hint() const;
    bool is_col_hint() const;
};

class PointingHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void
            find_block_hint(int block_idx, Grid &grid, HintConsumer &consumer) const;
    PointingHint *create_hint(Grid &grid, int start_row, int start_col, int value, int row,
            int col) const;
};
#endif /* POINTING_HPP_ */
