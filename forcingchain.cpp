#include <iostream>
#include <algorithm>
#include <memory>

#include "forcingchain.hpp"
#include "util.hpp"
#include "grid.hpp"
#include "range.hpp"
#include "hint.hpp"
#include "hintconsumer.hpp"

struct print_row_col {
    int idx;

    print_row_col(int idx) :
        idx(idx) {
    }

    void print(std::ostream &out) const {
        out << '(' << idx / 9 + 1 << ',' << idx % 9 + 1 << ')';
    }
};

inline std::ostream &operator <<(std::ostream &out,
        const print_row_col &row_col) {
    row_col.print(out);
    return out;
}

struct print_forcing_chain {
    const std::vector<Conclusion *> &conclusions;

    print_forcing_chain(const std::vector<Conclusion *> &conclusions) :
        conclusions(conclusions) {
    }

    void print(std::ostream &out) const {
        for (std::vector<Conclusion *>::const_iterator i = conclusions.begin(); i
                != conclusions.end(); ++i) {
            out << print_row_col((*i)->get_cell_idx()) << "="
                    << (*i)->get_value();
            if (i + 1 != conclusions.end())
                out << " => ";
        }
    }
};

inline std::ostream &operator <<(std::ostream &out,
        const print_forcing_chain &chain) {
    chain.print(out);
    return out;
}

Conclusion::Conclusion(Conclusion *parent, int cell_idx, int value) :
    parent(parent), cell_idx(cell_idx), value(value) {
}

Conclusion::~Conclusion() {
    std::for_each(conclusions.begin(), conclusions.end(),
            destroy<Conclusion *> ());
}

int Conclusion::get_cell_idx() const {
    return cell_idx;
}

int Conclusion::get_value() const {
    return value;
}

void Conclusion::add_conclusion(Conclusion *child) {
    conclusions.push_back(child);
}

Conclusion *Conclusion::get_parent() {
    return parent;
}

ForcingChainContradictionHint::ForcingChainContradictionHint(Grid &grid,
        int cell_idx, int value, int contradicted_cell_idx, const std::vector<
                Conclusion *> &first_chain,
        const std::vector<Conclusion *> &second_chain) :
    grid(grid), cell_idx(cell_idx), value(value), contradicted_cell_idx(
            contradicted_cell_idx), first_chain(first_chain), second_chain(
            second_chain) {

}

ForcingChainContradictionHint::~ForcingChainContradictionHint() {
    delete first_chain.front();
}

void ForcingChainContradictionHint::apply() {
    Cell &cell = grid[cell_idx];
    cell.set_value(value);
    grid.cleanup_choice(cell);
}

void ForcingChainContradictionHint::print_description(std::ostream &out) const {
    out << "contradiction: " << print_row_col(cell_idx) << " must have value: "
            << value << " first forcing chain: " << print_forcing_chain(
            first_chain) << " second forcing chain: " << print_forcing_chain(
            second_chain);
}

ForcingChainHint::ForcingChainHint(Grid &grid, int start_cell_idx,
        int cell_idx, int value, const std::vector<Conclusion *> &first_chain,
        const std::vector<Conclusion *> &second_chain) :
    grid(grid), start_cell_idx(start_cell_idx), cell_idx(cell_idx),
            value(value), first_chain(first_chain), second_chain(second_chain)

{
}

void ForcingChainHint::apply() {
    Cell &cell = grid[cell_idx];
    cell.set_value(value);
    grid.cleanup_choice(cell);
}

void ForcingChainHint::print_description(std::ostream &out) const {
    out << "forcing chain: start cell: " << print_row_col(start_cell_idx)
            << " cell: " << print_row_col(cell_idx) << " value: " << value
            << " first chain: " << print_forcing_chain(first_chain)
            << " second chain: " << print_forcing_chain(second_chain);
}

void ForcingChainHintProducer::find_next_cells(Grid &grid, int cell_idx,
        int value, std::vector<Cell *> &cells) const {
    RangeList::const_index_iterator begin = RANGES.field_begin(cell_idx);
    RangeList::const_index_iterator end = RANGES.field_end(cell_idx);

    for (RangeList::const_index_iterator i = begin; i != end; ++i) {
        Cell &cell = grid[*i];
        if (cell.get_num_choices() == 2 && cell.has_choice(value)
                && cell.get_idx() != cell_idx) {
            cells.push_back(&cell);
        }
    }
}

int ForcingChainHintProducer::get_concluded_choice(const Cell &cell, int value) const {
    for (int i = 1; i < 10; ++i) {
        if (i != value && cell.has_choice(i))
            return i;
    }
    return 0;
}

void ForcingChainHintProducer::get_choices(const Cell &cell,
        std::vector<int> &choices) const {
    for (int value = 1; value < 10; ++value)
        if (cell.has_choice(value))
            choices.push_back(value);
}

void ForcingChainHintProducer::fill_chain(Conclusion *conclusion, std::vector<
        Conclusion *> &chain) const {
    while (conclusion != 0) {
        chain.push_back(conclusion);
        conclusion = conclusion->get_parent();
    }
    std::reverse(chain.begin(), chain.end());
}

void ForcingChainHintProducer::consume_contradiction_hint(Grid &grid,
        Conclusion *original, Conclusion *contradiction, HintConsumer &consumer) const {
    std::vector<Conclusion *> first_chain;
    std::vector<Conclusion *> second_chain;

    fill_chain(original, first_chain);
    fill_chain(contradiction, second_chain);

    Conclusion *first_conclusion = first_chain.front();
    Cell &cell = grid[first_conclusion->get_cell_idx()];
    int value = get_concluded_choice(cell, first_conclusion->get_value());

    consumer.consume_hint(new ForcingChainContradictionHint(grid,
            first_conclusion->get_cell_idx(), value,
            contradiction->get_cell_idx(), first_chain, second_chain));
}

void ForcingChainHintProducer::consume_forcing_chain_hint(Grid &grid,
        Conclusion *first_match, Conclusion *second_match,
        HintConsumer &consumer) const {
    std::vector<Conclusion *> first_chain;
    std::vector<Conclusion *> second_chain;
    fill_chain(first_match, first_chain);
    fill_chain(second_match, second_chain);

    Conclusion *first_conclusion = first_chain.front();

    int cell_idx = first_match->get_cell_idx();
    int value = first_match->get_value();

    int start_cell_idx = first_conclusion->get_cell_idx();

    consumer.consume_hint(new ForcingChainHint(grid, start_cell_idx, cell_idx,
            value, first_chain, second_chain));
}

bool ForcingChainHintProducer::find_forcing_chain(Grid &grid,
        Conclusion *parent,
        std::map<int, std::vector<Conclusion *> > &concluded_values,
        HintConsumer &consumer) const {
    std::vector<Cell *> next_cells;
    find_next_cells(grid, parent->get_cell_idx(), parent->get_value(),
            next_cells);
    if (next_cells.empty())
        return true;
    for (std::vector<Cell *>::iterator i = next_cells.begin(); i
            != next_cells.end(); ++i) {
        int concluded_value = get_concluded_choice(**i, parent->get_value());
        std::map<int, std::vector<Conclusion *> >::iterator f =
                concluded_values.find((*i)->get_idx());
        if (f != concluded_values.end()) {
            if (f->second.front()->get_value() != concluded_value) {
                Conclusion *c = new Conclusion(parent, (*i)->get_idx(),
                        concluded_value);
                parent->add_conclusion(c);
                consume_contradiction_hint(grid, f->second.front(), c, consumer);
                return false;
            } else {
                continue;
            }
        }
        Conclusion *conclusion = new Conclusion(parent, (*i)->get_idx(),
                concluded_value);
        concluded_values[(*i)->get_idx()].push_back(conclusion);
        parent->add_conclusion(conclusion);
        if (!find_forcing_chain(grid, conclusion, concluded_values, consumer))
            return false;
    }

    return true;
}

bool ForcingChainHintProducer::find_forcing_chain_match(Grid &grid, std::map<
        int, std::vector<Conclusion *> > &first_conclusions, std::map<int,
        std::vector<Conclusion *> > &second_conclusions, HintConsumer &consumer) const {
    for (std::map<int, std::vector<Conclusion *> >::iterator ifirst =
            first_conclusions.begin(); ifirst != first_conclusions.end(); ++ifirst) {
        std::map<int, std::vector<Conclusion *> >::iterator isecond =
                second_conclusions.find(ifirst->first);
        if (isecond != second_conclusions.end()
                && isecond->second.front()->get_value()
                        == ifirst->second.front()->get_value()) {
            consume_forcing_chain_hint(grid, ifirst->second.front(),
                    isecond->second.front(), consumer);
            return true;
        }
    }
    return false;
}

void ForcingChainHintProducer::find_forcing_chains(Grid &grid,
        Cell &start_cell, HintConsumer &consumer) const {
    std::vector<int> choices;
    std::map<int, std::vector<Conclusion *> > first_concluded_values;

    get_choices(start_cell, choices);

    Conclusion *first_conclusion = new Conclusion(0, start_cell.get_idx(),
            choices[0]);
    first_concluded_values[start_cell.get_idx()].push_back(first_conclusion);

    if (!find_forcing_chain(grid, first_conclusion, first_concluded_values,
            consumer)) {
        return;
    }

    Conclusion *second_conclusion = new Conclusion(0, start_cell.get_idx(),
            choices[1]);

    std::map<int, std::vector<Conclusion *> > second_concluded_values;
    second_concluded_values[start_cell.get_idx()].push_back(second_conclusion);

    if (!find_forcing_chain(grid, second_conclusion, second_concluded_values,
            consumer)) {
        delete first_conclusion;
        return;
    }

    if (find_forcing_chain_match(grid, first_concluded_values,
            second_concluded_values, consumer)) {
        return;
    }

    delete first_conclusion;
    delete second_conclusion;
}

void ForcingChainHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    for (int i = 0; i < 81; ++i) {
        Cell &cell = grid[i];
        if (cell.get_num_choices() == 2) {
            find_forcing_chains(grid, cell, consumer);
            if (!consumer.wants_more_hints())
                break;
        }

    }
}
