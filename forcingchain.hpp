#ifndef FORCINGCHAIN_HPP
#define FORCINGCHAIN_HPP

#include <vector>
#include "hint.hpp"

class Range;
class Link;

class ForcingChainHint : public Hint {
    Grid &grid;
    std::vector<Link *> links;
    std::vector<std::vector<Link *> > chains;
public:
    ForcingChainHint(Grid &grid, std::vector<Link *> links);
    virtual ~ForcingChainHint();  
    void apply();
    void print_description(std::ostream &out) const;
};

class ForcingChainContradictionHint : public Hint {
    Grid &grid;
    Link *first_link;
    Link *second_link;
    std::vector<Link *> first_chain;
    std::vector<Link *> second_chain;
public:
    ForcingChainContradictionHint(Grid &grid, Link *first_link, Link *second_link);
    virtual ~ForcingChainContradictionHint();
    void apply();
    void print_description(std::ostream &out) const;
};

class Link {
public:
    typedef std::vector<Link *>::iterator iterator;
    typedef std::vector<Link *>::const_iterator const_iterator;
private:
    Link *parent;
    int  cell_idx;
    int  value;
    std::vector<Link *> children;
public:
    Link(Link *parent, int cell_idx, int value);
    virtual ~Link();
    const Link *get_parent() const;
    Link *get_parent();
    int get_cell_idx() const;
    int get_value() const;
    const_iterator begin() const;
    const_iterator end() const;
    virtual bool is_strong_link() const = 0;
private:
    Link(const Link *other) {}
    Link &operator = (const Link *other) { return *this; }
};

class StrongLink : public Link {
public:
    StrongLink(Link *parent, int cell_idx, int value);
    virtual bool is_strong_link() const;
};

class WeakLink : public Link {
public:
    WeakLink(Link *parent, int cell_idx, int value);
    virtual bool is_strong_link() const;
};

class LinkMap {
    std::vector<std::vector<Link *> > weak_links;
    std::vector<std::vector<Link *> > strong_links;
public:
    LinkMap();
    Link *find_contradiction(const Link *link);
    bool insert(Link *link);
    void insert_all(LinkMap &other);
    bool find_common_conclusion(size_t size, std::vector<Link *> &conclusions);
private:
    LinkMap(const LinkMap &other) {}
    LinkMap &operator = (const LinkMap &other) { return *this; }
    Link *find_strong_contradiction(const Link *link);
    Link *find_weak_contradiction(const Link *link);
    bool all_values_equal(const std::vector<Link *> &v) const;
};

class ForcingChainHintProducer : public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void find_forcing_chain(Cell &cell, Grid &grid, HintConsumer &consumer) const;
    bool find_contradiction(Link *link, LinkMap &linkMap, Grid &grid, HintConsumer &consumer) const;
    bool find_common_conclusion(LinkMap &allLinks, size_t nconclusions, Grid &grid, HintConsumer &consumer) const;
    void find_strong_links(Link *link, std::vector<Link *> &links, Grid &grid) const;
    void find_weak_links(Link *link, std::vector<Link *> &links, Grid &grid) const;
    void find_links_with_one_choice_left(Link *link, std::vector<Link *> &links, Grid &grid) const;
    void find_links_in_ranges(Link *link, std::vector<Link *> &links, Grid &grid) const;
    void fill_range_frequencies(const Range &range, Grid &grid, std::vector<std::vector<Cell *> > &frequencies) const;
};

#endif
