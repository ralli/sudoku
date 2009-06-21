#ifndef XYWING_HPP
#define XYWING_HPP

#include <vector>

#include "indirecthint.hpp"

class Grid;
class Cell;

class XYWingHint: public IndirectHint {
    const Cell &xy;
    const Cell &yz;
    const Cell &xz;
    int x;
    int y;
    int z;
public:
    XYWingHint(const Cell &xy, const Cell &yz, const Cell &xz, int x, int y,
            int z);
    void print_description(std::ostream &out) const;
};

class XYWingHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void find_xy_wing(Cell &cell, Grid &grid, HintConsumer &consumer) const;
    void fill_cell_values(const Cell &cell, int values[2]) const;
    XYWingHint *create_xy_hint(Cell &xy, Cell &yz, Cell &xz, int x, int y,
            int z, Grid &grid) const;
    bool shares_region(const Cell &a, const Cell &b) const;
    XYWingHint *create_hint(const Cell &xy, const Cell &yz, const Cell &xz, int x, int y,
            int z, Grid &grid, const std::vector<int> &cells_to_clear) const;
};

#endif
