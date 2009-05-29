#ifndef RANGE_HPP_
#define RANGE_HPP_

#include <string>
#include <vector>

class Range {
public:
    typedef const int *const_iterator;
public:
    std::string name;
    int indexes[9];
public:
    Range(const std::string &name, int indexes[9]);
    Range(const Range &other);
    Range & operator =(const Range &other);
    int operator[](int idx) const;
    int &operator[](int idx);
    const_iterator begin() const;
    const_iterator end() const;
    const std::string &get_name() const;
};

class RangeList {
public:
    typedef std::vector<Range>::iterator iterator;
    typedef std::vector<Range>::const_iterator const_iterator;
    typedef std::vector<int>::const_iterator const_index_iterator;
private:
    std::vector<Range> ranges;
    std::vector<std::vector<Range> > field_ranges;
    std::vector<std::vector<int> > field_neighbours;
    std::vector<Range> rows;
    std::vector<Range> columns;
    std::vector<Range> blocks;
public:
    RangeList();
    RangeList(const RangeList &other);
    RangeList & operator =(const RangeList &other);
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    const_index_iterator field_begin(int idx) const;
    const_index_iterator field_end(int idx) const;

    const Range &get_row(int row) const;
    const Range &get_column(int col) const;
    const Range &get_block(int block) const;
    const std::vector<Range> &get_field_ranges(int idx) const;
};

extern const RangeList RANGES;

inline Range::Range(const std::string &name, int indexes[9]) :
    name(name) {
    std::copy(indexes, indexes + 9, this->indexes);
}

inline Range::Range(const Range &other) :
    name(other.name) {

    std::copy(other.indexes, other.indexes + 9, indexes);
}

inline Range &Range::operator =(const Range &other) {
    if (this == &other)
        return *this;

    std::copy(other.indexes, other.indexes + 9, indexes);

    return *this;
}

inline int Range::operator[](int idx) const {
    return indexes[idx];
}

inline int &Range::operator[](int idx) {
    return indexes[idx];
}

inline Range::const_iterator Range::begin() const {
    return indexes;
}

inline Range::const_iterator Range::end() const {
    return indexes + 9;
}

inline const std::string &Range::get_name() const {
    return name;
}

inline RangeList::iterator RangeList::begin() {
    return ranges.begin();
}

inline RangeList::iterator RangeList::end() {
    return ranges.end();
}

inline RangeList::const_iterator RangeList::begin() const {
    return ranges.begin();
}

inline RangeList::const_iterator RangeList::end() const {
    return ranges.end();
}

inline RangeList::const_index_iterator RangeList::field_begin(int idx) const {
    return field_neighbours[idx].begin();
}

inline RangeList::const_index_iterator RangeList::field_end(int idx) const {
    return field_neighbours[idx].end();
}

inline const Range &RangeList::get_row(int row) const {
    return rows[row];
}

inline const Range &RangeList::get_column(int col) const {
    return columns[col];
}

inline const Range &RangeList::get_block(int block) const {
    return blocks[block];
}

inline const std::vector<Range> &RangeList::get_field_ranges(int idx) const 
{
    return field_ranges[idx];
}

#endif /* RANGE_HPP_ */
