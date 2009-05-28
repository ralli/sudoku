#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <iostream>

template<class T>
struct destroy {
    void operator()(T t) {
        delete t;
    }
};

struct print_row_col {
    int idx;

    print_row_col(int idx) :
        idx(idx) {
    }

    void print(std::ostream &out) const {
        out << '(' << idx / 9 + 1 << ',' << idx % 9 + 1 << ')';
    }
};

inline std::ostream &operator <<(std::ostream &out,
        const print_row_col &row_col) {
    row_col.print(out);
    return out;
}

#endif
