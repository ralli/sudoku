#include "claiming.hpp"
#include "hintconsumer.hpp"
#include "range.hpp"

ClaimingRowHint::ClaimingRowHint(std::vector<Cell *> &cells,int row, int block_idx, Grid &grid)
: cells(cells), row(row), block_idx(block_idx), grid(grid)
{
}

void ClaimingRowHint::apply() 
{
}

void ClaimingRowHint::print_description(std::ostream &out) const
{
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

int ClaimingHintProducer::find_block_by(int start_row, int start_col) const
{
    int idx = start_row * 3 + start_col;
    for(int i = 0; i < 9; ++i) {
        Range &block = RANGES.get_block(i);
        if(block[0] == idx)
            return i;
    }
    return 0;
}

void ClaimingHintProducer::find_row_hint(int row, Grid &grid, HintConsumer &consumer) const
{
    const Range &range = RANGES.get_row(row);
    for(int value = 1; value < 10; ++value) {
        std::vector<std::vector<Cell *> > block_cells(3);
        for(int i = 0; i < 9; ++i) {
            Cell &cell = grid[range[i]]
            cell.has_choice(value);
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
                    consumer.consume_hint(new ClaimingRowHint(block_cells[i], row, block_idx, grid); 
                    // found
                }
            }
        }
    }
}

void ClaimingHintProducer::find_col_hint(int col, Grid &grid, HintConsumer &consumer) const
{
}
