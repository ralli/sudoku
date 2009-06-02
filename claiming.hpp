#ifndef CLAIMING_HPP
#define CLAIMING_HPP

#include "hint.hpp"

class ClaimingRowHint : public Hint {
    std::vector<Cell *> cells;
    int                 row; 
    int                 block_idx;
    Grid                &grid;
public:
    ClaimingRowHint(std::vector<Cell *> &cells,int row, int block_idx, Grid &grid);
    void apply();
    void print_description(std::ostream &out) const;
};

class ClaimingHintProducer : public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
};

#endif