#ifndef XWING_HPP_
#define XWING_HPP_

#include <vector>
#include "hint.hpp"

class Cell;

class XWingRowHint: public Hint {
    Grid &grid;
    int row1;
    int row2;
    int col1;
    int col2;
    int value;
public:
    XWingRowHint(Grid &grid, int row1, int row2, int col1, int col2, int value);
    void apply();
    void print_description(std::ostream &out) const;
};

class XWingColumnHint: public Hint {
    Grid &grid;
    int row1;
    int row2;
    int col1;
    int col2;
    int value;
public:
    XWingColumnHint(Grid &grid, int row1, int row2, int col1, int col2,
            int value);
    void apply();
    void print_description(std::ostream &out) const;
};

class XWingHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    int count_col_value(Grid &grid, int col, int value) const;
    int count_row_value(Grid &grid, int row, int value) const;
    bool
            check_second_row(int row2, int col1, int col2, Grid & grid,
                    int value) const;
    bool
            check_second_col(int col, int row1, int row2, Grid & grid,
                    int value) const;
    void find_row_hints(Grid &grid, HintConsumer &consumer) const;
    void find_col_hints(Grid &grid, HintConsumer &consumer) const;
    void fill_row_freq(Grid &grid, int row, std::vector<std::vector<Cell *> > &freq) const;
    void fill_col_freq(Grid &grid, int col, std::vector<std::vector<Cell *> > &freq) const;
};

#endif
