#include <iostream>

#include "claiming.hpp"
#include "hintconsumer.hpp"
#include "range.hpp"
#include "grid.hpp"

ClaimingRowHint::ClaimingRowHint(std::vector<Cell *> &cells,int row, int block_idx, int value, Grid &grid)
: cells(cells), row(row), block_idx(block_idx), value(value), grid(grid)
{
}

void ClaimingRowHint::apply() 
{
    std::vector<bool> cell_flags(81);
    for(std::vector<Cell *>::const_iterator i = cells.begin(); i != cells.end(); ++i) 
        cell_flags[(*i)->get_idx()] = true;
    const Range &block = RANGES.get_block(block_idx);
    for(int i = 0; i < 9; ++i) {
        int idx = block[i];
        if(!cell_flags[idx]) {
            Cell &cell = grid[idx];

            cell.remove_choice(value);
        }
    }
}

void ClaimingRowHint::print_description(std::ostream &out) const
{
    const Range &block = RANGES.get_block(block_idx);
    out << "claiming(row): row: " 
	<< row + 1 
	<< " block: " << block.get_name() 
	<< " value: " << value;
}

void ClaimingHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    for(int row = 0; row < 9; ++row) {
        find_row_hint(row, grid, consumer);
        if(!consumer.wants_more_hints())
            return;
    }

    for(int col = 0; col < 9; ++col) {
        find_col_hint(col, grid, consumer);
        if(!consumer.wants_more_hints())
            return;
    }
}

ClaimingColumnHint::ClaimingColumnHint(std::vector<Cell *> &cells,int col, int block_idx, int value, Grid &grid) : cells(cells),
col(col), block_idx(block_idx), value(value), grid(grid)
{
}

void ClaimingColumnHint::apply()
{
    std::vector<bool> cell_flags(81);
    for(std::vector<Cell *>::const_iterator i = cells.begin(); i != cells.end(); ++i) 
        cell_flags[(*i)->get_idx()] = true;
    const Range &block = RANGES.get_block(block_idx);
    for(int i = 0; i < 9; ++i) {
        int idx = block[i];
        if(!cell_flags[idx]) {
            Cell &cell = grid[idx];

            cell.remove_choice(value);
        }
    }
}

void ClaimingColumnHint::print_description(std::ostream &out) const
{
    const Range &block = RANGES.get_block(block_idx);
    out << "claiming(col): col: " << col+1<< " block: " << block.get_name() << " value: " << value;
}

int ClaimingHintProducer::find_block_by(int start_row, int start_col) const
{
    int idx = start_row * 9 + start_col;
    for(int i = 0; i < 9; ++i) {
        const Range &block = RANGES.get_block(i);
        if(block[0] == idx)
            return i;
    }
    return 0;
}

bool ClaimingHintProducer::check_block(const std::vector<Cell *> &cells, int block_idx, int value, Grid &grid) const
{
    const Range &block = RANGES.get_block(block_idx);
    std::vector<bool> cell_flags(81);
    for(std::vector<Cell *>::const_iterator i = cells.begin(); i != cells.end(); ++i) {
        cell_flags[(*i)->get_idx()] = true;
    }

    for(int i = 0; i < 9; ++i) {
        int idx = block[i];
        const Cell &cell = grid[idx];
        if(cell_flags[idx] == false && cell.has_choice(value))
            return true;
    }

    return false;
}

void ClaimingHintProducer::find_row_hint(int row, Grid &grid, HintConsumer &consumer) const
{
    const Range &range = RANGES.get_row(row);
    for(int value = 1; value < 10; ++value) {
        std::vector<std::vector<Cell *> > block_cells(3);
        for(int i = 0; i < 9; ++i) {
            Cell &cell = grid[range[i]];
            if(cell.has_choice(value))
                block_cells[i / 3].push_back(&cell);
        }
        int count = 0;
        for(int i = 0; i < 3; ++i) {
            if(block_cells[i].size() > 0)
                ++count;
        }
        if(count == 1) {
            for(int i = 0; i < 3; ++i) {
                if(block_cells[i].size() > 0) {
                    int start_row = row / 3 * 3;
                    int start_col = i * 3;
                    int block_idx = find_block_by(start_row, start_col);
                    if(!check_block(block_cells[i], block_idx, value, grid))
                        continue;
                    if(!consumer.consume_hint(new ClaimingRowHint(block_cells[i], row, block_idx, value, grid)))
                        return;
                }
            }
        }
    }
}


void ClaimingHintProducer::find_col_hint(int col, Grid &grid, HintConsumer &consumer) const
{
    const Range &range = RANGES.get_column(col);
    for(int value = 1; value < 10; ++value) {
        std::vector<std::vector<Cell *> > block_cells(3);
        for(int i = 0; i < 9; ++i) {
            Cell &cell = grid[range[i]];
            if(cell.has_choice(value))
                block_cells[i / 3].push_back(&cell);
        }
        int count = 0;
        for(int i = 0; i < 3; ++i) {
            if(block_cells[i].size() > 0)
                ++count;
        }
        if(count == 1) {
            for(int i = 0; i < 3; ++i) {
                if(block_cells[i].size() > 0) {
                    int start_row = i * 3;
                    int start_col = col / 3 * 3;
                    int block_idx = find_block_by(start_row, start_col);
                    if(!check_block(block_cells[i], block_idx, value, grid))
                        continue;
                    if(!consumer.consume_hint(new ClaimingColumnHint(block_cells[i], col, block_idx, value, grid)))
                        return;                    
                }
            }
        }
    }
}
