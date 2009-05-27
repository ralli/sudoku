#ifndef BOXLINEREDUCTION_HPP_
#define BOXLINEREDUCTION_HPP_

#include "hint.hpp"
#include <set>
#include <vector>

class BoxLineRowReductionHint: public Hint {
    Grid &grid;
    int value;
    int first_block;
    int second_block;
    int third_block;
    int row_to_remove1;
    int row_to_remove2;
    int row_to_keep;
public:
    BoxLineRowReductionHint(Grid &grid, int value, int first_block,
            int second_block, int third_block, int rowcol_to_remove1,
            int rowcol_to_remove2, int rowcol_to_keep);
    void apply();
    void print_description(std::ostream &out) const;
};

class BoxLineColumnReductionHint: public Hint {
    Grid &grid;
    int value;
    int first_block;
    int second_block;
    int third_block;
    int col_to_remove1;
    int col_to_remove2;
    int col_to_keep;
public:
    BoxLineColumnReductionHint(Grid &grid, int value, int first_block,
            int second_block, int third_block, int col_to_remove1,
            int col_to_remove2, int col_to_keep);
    void apply();
    void print_description(std::ostream &out) const;
};

class BoxLineReductionHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void find_row_lines(Grid &grid, HintConsumer &consumer) const;
    void fill_intersection(const std::set<int> first,
            const std::set<int> &second, std::vector<int> &intersection,
            std::vector<int> &difference, std::vector<int> &rowunion) const;
    void find_row_line(Grid &grid, int start_row, HintConsumer &consumer) const;
    void find_row_col_block(Grid &grid, int row, int col_idx, int value,
            std::set<int> &rowset) const;
    void find_col_lines(Grid &grid, HintConsumer &consumer) const;
    void process_sets(Grid &grid, int row, int value,
            std::vector<std::set<int> > rowsets, bool isrow,
            HintConsumer &consumer) const;
    void find_col_line(Grid &grid, int col_idx, HintConsumer &consumer) const;
    void find_col_row_block(Grid &grid, int col, int row_idx, int value,
            std::set<int> &colset) const;
};

#endif
