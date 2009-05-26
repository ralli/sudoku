#include <iostream>
#include <algorithm>
#include <vector>

#include "hintconsumer.hpp"
#include "util.hpp"
#include "hint.hpp"


ListHintConsumer::~ListHintConsumer() {
    std::for_each(begin(), end(), destroy<Hint *> ());
}

void ListHintConsumer::clear() {
    std::for_each(begin(), end(), destroy<Hint *> ());
    hints.clear();
}
SingleHintConsumer::SingleHintConsumer() :
    success(false) {
}

bool SingleHintConsumer::consume_hint(Hint *hint) {
    hint->print_description(std::cout);
    std::cout << std::endl;
    hint->apply();
    delete hint;
    success = true;
    return false;
}

bool SingleHintConsumer::has_hints() const {
    return success;
}
