#include <iostream>

#include "range.hpp"
#include "grid.hpp"

void Cell::print_choices(std::ostream &out) const {
    int count = 0;

    if (has_value()) {
        out << get_value();
        ++count;
    } else {
        for (int i = 1; i < 10; ++i) {
            if (has_choice(i)) {
                out << i;
                ++count;
            }
        }
    }

    for (; count < 10; ++count) {
        out << ' ';
    }
}

void Grid::load(std::istream &in) {
    char ch;
    in.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);

    init_cells();

    for (int i = 0; i < 81; ++i) {
        in >> ch;
        int v = isdigit(ch) ? ch - '0' : 0;
        cells[i].set_value(v);
    }

    cleanup_choices();
}

void Grid::print(std::ostream &out) const {
    int i;
    int row;
    int col;

    i = 0;
    for (row = 0; row < 9; ++row) {
        if (row == 3 || row == 6) {
            out << "---+---+---" << std::endl;
            ;
        }
        for (col = 0; col < 9; ++col) {
            if (col == 3 || col == 6)
                out << '|';
            if (cells[i].has_value())
                out << cells[i].get_value();
            else
                out << '.';
            ++i;
        }
        out << std::endl;
    }
}

void Grid::print_choices(std::ostream &out) const {
    int i;
    int row;
    int col;

    i = 0;
    for (row = 0; row < 9; ++row) {
        for (col = 0; col < 9; ++col) {
            cells[i].print_choices(out);
            ++i;
        }
        out << std::endl;
    }
}

void Grid::print_status(std::ostream &out) const {
    out << "Choices: " << get_num_choices() << " todo: " << get_to_do()
            << std::endl;
}

void Grid::cleanup_choice(Cell &cell) {
    if (!cell.has_value()) {
        return;
    }

    cell.clear_choices();

    RangeList::const_index_iterator begin = RANGES.field_begin(cell.get_idx());
    RangeList::const_index_iterator end = RANGES.field_end(cell.get_idx());

    for (RangeList::const_index_iterator j = begin; j != end; ++j) {
        cells[*j].remove_choice(cell.get_value());
    }
}
