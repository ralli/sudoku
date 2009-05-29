#ifndef FORCINGCHAIN_HPP_
#define FORCINGCHAIN_HPP_

#include <vector>
#include <map>
#include "hint.hpp"

class Cell;
class Grid;
class HintConsumer;

class SimpleConclusion {
    SimpleConclusion *parent;
    int cell_idx;
    int value;
    std::vector<SimpleConclusion *> conclusions;
public:
    SimpleConclusion(SimpleConclusion *parent, int cell_idx, int value);
    virtual ~SimpleConclusion();
    int get_cell_idx() const;
    int get_value() const;
    void add_conclusion(SimpleConclusion *child);
    SimpleConclusion *get_parent();
private:
    SimpleConclusion(const SimpleConclusion &other) {
    }
    SimpleConclusion &operator =(const SimpleConclusion &other) {
        return *this;
    }
};

class SimpleForcingChainContradictionHint: public Hint {
    Grid &grid;
    int cell_idx;
    int value;
    int contradicted_cell_idx;
    std::vector<SimpleConclusion *> first_chain;
    std::vector<SimpleConclusion *> second_chain;
public:
    SimpleForcingChainContradictionHint(Grid &grid, int cell_idx, int value,
            int contradicted_cell_idx,
            const std::vector<SimpleConclusion *> &first_chain, const std::vector<
                    SimpleConclusion *> &second_chain);
    virtual ~SimpleForcingChainContradictionHint();
    void apply();
    void print_description(std::ostream &out) const;
};

class SimpleForcingChainHint: public Hint {
    Grid &grid;
    int start_cell_idx;
    int cell_idx;
    int value;
    std::vector<SimpleConclusion *> first_chain;
    std::vector<SimpleConclusion *> second_chain;
public:
    SimpleForcingChainHint(Grid &grid, int start_cell_idx, int cell_idx, int value,
            const std::vector<SimpleConclusion *> &first_chain, const std::vector<
                    SimpleConclusion *> &second_chain);
    void apply();
    void print_description(std::ostream &out) const;
};

class SimpleForcingChainHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void find_next_cells(Grid &grid, int cell_idx,
            int value, std::vector<Cell *> &cells) const;
    int get_concluded_choice(const Cell &cell, int value) const;
    void get_choices(const Cell &cell, std::vector<int> &choices) const;
    void
            fill_chain(SimpleConclusion *conclusion, std::vector<SimpleConclusion *> &chain) const;
    void consume_contradiction_hint(Grid &grid, SimpleConclusion *original,
            SimpleConclusion *contradiction, HintConsumer &consumer) const;
    void consume_forcing_chain_hint(Grid &grid, SimpleConclusion *first_match,
            SimpleConclusion *second_match, HintConsumer &consumer) const;
    bool find_forcing_chain(Grid &grid, SimpleConclusion *parent, std::map<int,
            std::vector<SimpleConclusion *> > &concluded_values,
            HintConsumer &consumer) const;
    bool find_forcing_chain_match(Grid &grid, std::map<
            int, std::vector<SimpleConclusion *> > &first_conclusions, std::map<int,
            std::vector<SimpleConclusion *> > &second_conclusions, HintConsumer &consumer) const;
    void find_forcing_chains(Grid &grid, Cell &start_cell, HintConsumer &consumer) const;
};

#endif
