#include <algorithm>
#include <cstdlib>
#include "sudokugenerator.hpp"
#include "grid.hpp"
#include "hint.hpp"
#include "util.hpp"
#include "hintconsumer.hpp"
#include "nakedsingle.hpp"
#include "singlehint.hpp"
#include "range.hpp"
#include "gridchecker.hpp"

class MyHintConsumer: public HintConsumer {
    bool success;
public:
    MyHintConsumer() :
        success(false) {
    }

    bool consume_hint(Hint *hint) {
        hint->apply();
        delete hint;
        success = true;
        return false;
    }
    bool wants_more_hints() const {
        return !success;
    }
};

SudokuGenerator::SudokuGenerator() {
    hint_producers.push_back(new NakedSingleHintProducer());
    hint_producers.push_back(new SingleHintProducer());
}

SudokuGenerator::~SudokuGenerator() {
    std::for_each(hint_producers.begin(), hint_producers.end(), destroy<
            HintProducer *> ());
}

void SudokuGenerator::generate(Grid &grid) {
    do {
        do_generate(grid);
    } while (!check_difficulty(grid));
}

bool SudokuGenerator::check_difficulty(const Grid &testgrid) {
    Grid grid = testgrid;
    while (grid.get_to_do() > 0) {
        bool success = false;
        for (std::vector<HintProducer *>::iterator i = hint_producers.begin(); i
                != hint_producers.end(); ++i) {
            MyHintConsumer consumer;
            (*i)->find_hints(grid, consumer);
            if(!consumer.wants_more_hints()) {
                success = true;
                break;
            }
        }
        if(!success)
            return false;
    }
    return true;
}

void SudokuGenerator::do_generate(Grid &grid) {
    std::fill(field, field + 81, 0);
    generate_full(0);
    for (Grid::iterator i = grid.begin(); i != grid.end(); ++i) {
        Cell &cell = *i;
        cell.set_value(field[cell.get_idx()]);
        cell.clear_choices();
    }
    remove_fields(grid);
}

bool SudokuGenerator::generate_full(int idx) {
    if (idx == 81)
        return true;

    Choices choices;
    fill_choices(idx, choices);
    int choice;
    while ((choice = random_choice(choices)) != 0) {
        choices.remove_choice(choice);
        field[idx] = choice;
        if (generate_full(idx + 1))
            return true;
    }
    field[idx] = 0;
    return false;
}

void SudokuGenerator::fill_choices(int idx, Choices &choices) const {
    RangeList::const_index_iterator begin = RANGES.field_begin(idx);
    RangeList::const_index_iterator end = RANGES.field_end(idx);
    for (RangeList::const_index_iterator i = begin; i != end; ++i) {
        if (field[*i]) {
            choices.remove_choice(field[*i]);
        }
    }
}

int SudokuGenerator::random_choice(Choices &choices) const {
    int num_choices = choices.get_num_choices();
    if (num_choices == 0)
        return 0;
    int idx = rand() % choices.get_num_choices() + 1;
    int i = 0;

    for (int value = 1; value <= 9; ++value) {
        if (choices.has_choice(value)) {
            ++i;
            if (i == idx) {
                return value;
            }
        }
    }

    return 0;
}

void SudokuGenerator::remove_fields(Grid &grid) const {
    GridChecker checker;
    const int min_filled_cells = 20;
    Grid backup = grid;
    bool used[81];
    int unused_count = 81;
    int filled_count = 81;

    std::fill(used, used + 81, false);
    while (filled_count > min_filled_cells && unused_count > 1) {
        int idx = rand() % 81;

        while (used[idx]) {
            ++idx;
            if (idx == 81)
                idx = 0;
        }

        used[idx] = true;
        --unused_count;

        grid.clear_cell_value(grid[idx]);
        if (!checker.check(grid)) {
            Cell &cell = grid[idx];
            cell.set_value(backup[idx].get_value());
            grid.cleanup_choice(cell);
        } else {
            --filled_count;
        }
    }
}
