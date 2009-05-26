#ifndef UTIL_HPP_
#define UTIL_HPP_

template<class T>
struct destroy {
    void operator()(T t) {
        delete t;
    }
};

#endif
