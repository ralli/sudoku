#ifndef INDIRECTHINT_HPP_
#define INDIRECTHINT_HPP_

#include "hint.hpp"
#include <vector>
#include <utility>

class Cell;

class IndirectHint: public Hint {
    std::vector<std::pair<Cell *, int> > choices_to_remove;
public:
    void add_choice_to_remove(Cell *cell, int value);
    const std::vector<std::pair<Cell *, int> > &get_choices_to_remove() const;
    void apply();
};

inline void IndirectHint::add_choice_to_remove(Cell *cell, int value) {
    choices_to_remove.push_back(std::pair<Cell*, int>(cell, value));
}

inline const std::vector<std::pair<Cell *, int> > &IndirectHint::get_choices_to_remove() const {
    return choices_to_remove;
}

struct print_choices_to_remove {
    const std::vector<std::pair<Cell *, int> > &choices_to_remove;

    print_choices_to_remove(
            const std::vector<std::pair<Cell *, int> > &choices_to_remove) :
        choices_to_remove(choices_to_remove) {

    }

    void print(std::ostream &out) const;
};

std::ostream &operator <<(std::ostream &out, const print_choices_to_remove &x);

#endif /* INDIRECTHINT_HPP_ */
