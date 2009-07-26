/*
 * Copyright (c) 2009, Ralph Juhnke
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following conditions
 * are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *    * Neither the name of "Ralph Juhnke" nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
    const char *get_name() const;
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
    const char *get_name() const;
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
