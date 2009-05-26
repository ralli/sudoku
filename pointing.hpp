#ifndef POINTING_HPP_
#define POINTING_HPP_

#include "hint.hpp"

class PointingHint: public Hint {
    Grid &grid;
    int start_row;
    int start_col;
    int value;
    int row;
    int col;
public:
    PointingHint(Grid &grid, int start_row, int start_col, int value, int row,
            int col);
    void apply();
    void print_description(std::ostream &out) const;
private:
    bool is_row_hint() const;
    bool is_col_hint() const;
    void apply_row_hint();
    void apply_col_hint();
};

class PointingHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void
            find_block_hint(int block_idx, Grid &grid, HintConsumer &consumer) const;
};
#endif /* POINTING_HPP_ */
