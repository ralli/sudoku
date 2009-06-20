#include "indirecthint.hpp"
#include "grid.hpp"
#include "util.hpp"

void IndirectHint::apply() {
    for (std::vector<std::pair<Cell *, int> >::iterator i =
            choices_to_remove.begin(); i != choices_to_remove.end(); ++i) {
        i->first->remove_choice(i->second);
    }
}
void print_choices_to_remove::print(std::ostream &out) const {
    for(std::vector<std::pair<Cell *, int> >::const_iterator i =
            choices_to_remove.begin(); i != choices_to_remove.end(); ++i) {
        out << print_row_col(i->first->get_idx()) << "=" << i->second;
        if((i+1) != choices_to_remove.end())
            out << ' ';
    }
}

std::ostream &operator << (std::ostream &out, const print_choices_to_remove &x) {
    x.print(out);
    return out;
}
