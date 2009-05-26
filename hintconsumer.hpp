#ifndef HINTCONSUMER_HPP_
#define HINTCONSUMER_HPP_

#include <vector>
#include <algorithm>

class Hint;

class HintConsumer {
public:
    virtual bool consume_hint(Hint *hint) = 0;
    virtual bool wants_more_hints() const = 0;
};

class ListHintConsumer: public HintConsumer {
public:
    typedef std::vector<Hint *>::iterator iterator;
    typedef std::vector<Hint *>::const_iterator const_iterator;
private:
    std::vector<Hint *> hints;
public:
    ListHintConsumer();
    virtual ~ListHintConsumer();
    bool consume_hint(Hint *hint);
    bool has_hints() const;
    void clear();
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    bool wants_more_hints() const  {
        return true;
    }
private:
    ListHintConsumer(const ListHintConsumer &other) {
    }

    ListHintConsumer & operator =(const ListHintConsumer &other) {
        return *this;
    }
};

class SingleHintConsumer: public HintConsumer {
private:
    bool success;
public:
    SingleHintConsumer();
    bool consume_hint(Hint *hint);
    bool has_hints() const;
    bool wants_more_hints() const  {
        return !success;
    }
private:
    SingleHintConsumer(const SingleHintConsumer &other) {
    }
    SingleHintConsumer &operator =(const SingleHintConsumer &other) {
        return *this;
    }
};

inline ListHintConsumer::ListHintConsumer() {

}



inline bool ListHintConsumer::consume_hint(Hint *hint) {
    hints.push_back(hint);
    return true;
}

inline bool ListHintConsumer::has_hints() const {
    return hints.size() > 0;
}


inline ListHintConsumer::iterator ListHintConsumer::begin() {
    return hints.begin();
}

inline ListHintConsumer::iterator ListHintConsumer::end() {
    return hints.end();
}

inline ListHintConsumer::const_iterator ListHintConsumer::begin() const {
    return hints.begin();
}

inline ListHintConsumer::const_iterator ListHintConsumer::end() const {
    return hints.end();
}

#endif /* HINTCONSUMER_HPP_ */
