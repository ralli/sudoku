#ifndef HINT_HPP_
#define HINT_HPP_

#include <iosfwd>

class HintConsumer;
class Grid;

class Hint {
public:
    Hint() {
    }

    virtual ~Hint() {
    }
    virtual void apply() = 0;
    virtual void print_description(std::ostream &out) const = 0;
private:

    Hint(const Hint &other) {
    }

    Hint & operator =(const Hint &other) {
        return *this;
    }
};


class HintProducer {
public:
    virtual void find_hints(Grid &grid, HintConsumer &consumer) = 0;
};

#endif
