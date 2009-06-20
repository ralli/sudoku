#ifndef HINTCONSUMER_HPP_
#define HINTCONSUMER_HPP_

#include <vector>
#include <algorithm>

#include "hint.hpp"

class SingleHintConsumer: public HintConsumer {
private:
    bool success;
public:
    SingleHintConsumer();
    bool consume_hint(Hint *hint);
    bool has_hints() const;
    bool wants_more_hints() const;
private:
    SingleHintConsumer(const SingleHintConsumer &other) {
    }

    SingleHintConsumer &operator =(const SingleHintConsumer &other) {
        return *this;
    }
};

inline bool SingleHintConsumer::wants_more_hints() const {
    return !success;
}

inline bool SingleHintConsumer::has_hints() const {
    return success;
}

#endif /* HINTCONSUMER_HPP_ */
