#ifndef HINT_HPP_
#define HINT_HPP_

#include <iosfwd>

class HintConsumer;
class Grid;

/*!
 * a hint is a single step (move or whatever)
 * to the solution of a sudoku.
 */

class Hint {
public:
    Hint() {
    }

    virtual ~Hint() {
    }

    virtual void apply() = 0;

    virtual void print_description(std::ostream &out) const = 0;

    virtual void visit_consumer(HintConsumer *consumer) const;
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

class HintConsumer {
public:
    virtual bool consume_hint(Hint *hint) = 0;
    virtual bool wants_more_hints() const = 0;
};

#endif
