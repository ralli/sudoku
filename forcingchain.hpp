#ifndef FORCINGCHAIN_HPP_
#define FORCINGCHAIN_HPP_

#include <vector>
#include <map>
#include "hint.hpp"

class Cell;
class Grid;
class HintConsumer;

class Conclusion {
    Conclusion *parent;
    int cell_idx;
    int value;
    std::vector<Conclusion *> conclusions;
public:
    Conclusion(Conclusion *parent, int cell_idx, int value);
    virtual ~Conclusion();
    int get_cell_idx() const;
    int get_value() const;
    void add_conclusion(Conclusion *child);
    Conclusion *get_parent();
private:
    Conclusion(const Conclusion &other) {
    }
    Conclusion &operator =(const Conclusion &other) {
        return *this;
    }
};

class ForcingChainContradictionHint: public Hint {
    Grid &grid;
    int cell_idx;
    int value;
    int contradicted_cell_idx;
    std::vector<Conclusion *> first_chain;
    std::vector<Conclusion *> second_chain;
public:
    ForcingChainContradictionHint(Grid &grid, int cell_idx, int value,
            int contradicted_cell_idx,
            const std::vector<Conclusion *> &first_chain, const std::vector<
                    Conclusion *> &second_chain);
    virtual ~ForcingChainContradictionHint();
    void apply();
    void print_description(std::ostream &out) const;
};

class ForcingChainHint: public Hint {
    Grid &grid;
    int start_cell_idx;
    int cell_idx;
    int value;
    std::vector<Conclusion *> first_chain;
    std::vector<Conclusion *> second_chain;
public:
    ForcingChainHint(Grid &grid, int start_cell_idx, int cell_idx, int value,
            const std::vector<Conclusion *> &first_chain, const std::vector<
                    Conclusion *> &second_chain);
    void apply();
    void print_description(std::ostream &out) const;
};

class ForcingChainHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void find_next_cells(Grid &grid, int cell_idx,
            int value, std::vector<Cell *> &cells) const;
    int get_concluded_choice(const Cell &cell, int value) const;
    void get_choices(const Cell &cell, std::vector<int> &choices) const;
    void
            fill_chain(Conclusion *conclusion, std::vector<Conclusion *> &chain) const;
    void consume_contradiction_hint(Grid &grid, Conclusion *original,
            Conclusion *contradiction, HintConsumer &consumer) const;
    void consume_forcing_chain_hint(Grid &grid, Conclusion *first_match,
            Conclusion *second_match, HintConsumer &consumer) const;
    bool find_forcing_chain(Grid &grid, Conclusion *parent, std::map<int,
            std::vector<Conclusion *> > &concluded_values,
            HintConsumer &consumer) const;
    bool find_forcing_chain_match(Grid &grid, std::map<
            int, std::vector<Conclusion *> > &first_conclusions, std::map<int,
            std::vector<Conclusion *> > &second_conclusions, HintConsumer &consumer) const;
    void find_forcing_chains(Grid &grid, Cell &start_cell, HintConsumer &consumer) const;
};

#endif
