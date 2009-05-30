#include <iostream>
#include <algorithm>
#include "grid.hpp"
#include "hintconsumer.hpp"
#include "forcingchain.hpp"
#include "util.hpp"
#include "range.hpp"

inline void fill_chain(Link *link, std::vector<Link *> &chain) {
    while(link) {
        chain.push_back(link);
        link = link->get_parent();
    }

    std::reverse(chain.begin(), chain.end());
}

struct print_link {
    const Link *link;

    print_link(const Link *link) : link(link) {}
    void print(std::ostream &out) const {
        out << print_row_col(link->get_cell_idx());
        if(link->is_strong_link())
            out << "=" ;
        else
            out << "<>";
        out << link->get_value();
    }
};

std::ostream &operator << (std::ostream &out, const print_link &pl) {
    pl.print(out);
    return out;
}

struct print_chain {
    const std::vector<Link *> &links;
    print_chain(const std::vector<Link *> &links) : links(links) {}
    void print(std::ostream &out) const {
        for(std::vector<Link *>::const_iterator i = links.begin(); i != links.end(); ++i) {
            out << print_link(*i);
            if(i+1 != links.end()) {
                out << " => ";
            }
        }
    }
};

std::ostream &operator << (std::ostream &out, const print_chain &chain) {
    chain.print(out);
    return out;
}

ForcingChainHint::ForcingChainHint(Grid &grid, std::vector<Link *> links) : grid(grid), links(links), chains(links.size()) {
    for(size_t i = 0; i < links.size(); ++i) {
        fill_chain(links[i], chains[i]);
    }
}

ForcingChainHint::~ForcingChainHint() {
    for(std::vector<std::vector<Link *> >::iterator i = chains.begin(); i != chains.end(); ++i)
        delete i->front();
}

void ForcingChainHint::apply() {
    Link *link = links.front();
    Cell &cell = grid[link->get_cell_idx()];
   
    if(link->is_strong_link()) {
        cell.set_value(link->get_value());
        grid.cleanup_choice(cell);
    }
    else {
        cell.remove_choice(link->get_value());
    }
}

void ForcingChainHint::print_description(std::ostream &out) const {
    out << "forcing chain: conclusion: " << print_link(links.front());
    out << " start cell: " << print_row_col(chains.front().front()->get_cell_idx());
    for(size_t i = 0; i < chains.size(); ++i) {
        out << " chain " << i + 1 << ": " << print_chain(chains[i]); 
    }
}

ForcingChainContradictionHint::ForcingChainContradictionHint(Grid &grid, Link *first_link, Link *second_link) 
: grid(grid), first_link(first_link), second_link(second_link)
{
    fill_chain(first_link, first_chain);
    fill_chain(second_link, second_chain);
}

ForcingChainContradictionHint::~ForcingChainContradictionHint()
{
    delete first_chain.front();
}

void ForcingChainContradictionHint::apply()
{
    Link *link = first_chain.front();
    Cell &cell = grid[link->get_cell_idx()];
    cell.remove_choice(link->get_value());
}

void ForcingChainContradictionHint::print_description(std::ostream &out) const
{
    Link *link = first_chain.front();
    out << "contradiction: cell: " << print_row_col(link->get_cell_idx()) << " cannot have value " << link->get_value();
    out << " first chain: " << print_chain(first_chain);
    out << " second chain: " << print_chain(second_chain);
}

Link::Link(Link *parent, int cell_idx, int value) 
: parent(parent), cell_idx(cell_idx), value(value)
{
    if(parent)
        parent->children.push_back(this);
}

Link::~Link() 
{
    std::for_each(children.begin(), children.end(), destroy<Link *>());
}

const Link *Link::get_parent() const 
{
    return parent;
}

Link *Link::get_parent() {
    return parent;
}

int Link::get_cell_idx() const 
{
    return cell_idx;
}

int Link::get_value() const 
{
    return value;
}

Link::const_iterator Link::begin() const 
{
    return children.begin();
}

Link::const_iterator Link::end() const 
{
    return children.end();
}

StrongLink::StrongLink(Link *parent, int cell_idx, int value) 
: Link(parent, cell_idx, value)
{
}

bool StrongLink::is_strong_link() const {
    return true;
}

WeakLink::WeakLink(Link *parent, int cell_idx, int value) 
: Link(parent, cell_idx, value)
{
}

bool WeakLink::is_strong_link() const {
    return false;
}

LinkMap::LinkMap() : strong_links(81), weak_links(81)
{
}

Link *LinkMap::find_contradiction(const Link *link)
{
    if(link->is_strong_link())
        return find_strong_contradiction(link);
    else
        return find_weak_contradiction(link);
}

Link *LinkMap::find_strong_contradiction(const Link *link)
{
    for(std::vector<Link *>::iterator i = strong_links[link->get_cell_idx()].begin(); i != strong_links[link->get_cell_idx()].end(); ++i) {
        if((*i)->get_value() != link->get_value())
            return *i;
    }

    for(std::vector<Link *>::iterator i = weak_links[link->get_cell_idx()].begin(); i != weak_links[link->get_cell_idx()].end(); ++i) {
        if((*i)->get_value() == link->get_value())
            return *i;
    }

    return 0;
}

Link *LinkMap::find_weak_contradiction(const Link *link)
{
    std::vector<Link *> &links = strong_links[link->get_cell_idx()];

    for(std::vector<Link *>::iterator i = links.begin(); i != links.end(); ++i) {
        if((*i)->get_value() == link->get_value())
            return *i;
    }

    return 0;
}

inline bool insert_into(std::vector<std::vector<Link *> > &linkmap, Link *link) {
    std::vector<Link *> &links = linkmap[link->get_cell_idx()];

    for(std::vector<Link *>::iterator i = links.begin(); i != links.end(); ++i) {
        if((*i)->get_value() == link->get_value())
            return false;
    }
    links.push_back(link);
    return true;
}

bool LinkMap::insert(Link *link)
{
    if(link->is_strong_link()) {
        return insert_into(strong_links, link);
    }
    else {
        return insert_into(weak_links, link);
    }
}

void LinkMap::insert_all(LinkMap &other) 
{
    for(int i = 0; i < 81; ++i) {
        std::copy(other.strong_links[i].begin(), other.strong_links[i].end(), std::back_inserter(strong_links[i]));
    }

    for(int i = 0; i < 81; ++i) {
        std::copy(other.weak_links[i].begin(), other.weak_links[i].end(), std::back_inserter(weak_links[i]));
    }
}

/**
** todo: find the common conclusion with the shortest forcing chains ...
** or find all of them ...
*/

bool LinkMap::find_common_conclusion(size_t size, std::vector<Link *> &conclusions) {
    for(int i = 0; i < 81; ++i) {
        if(strong_links[i].size() == size && all_values_equal(strong_links[i])) {
            std::copy(strong_links[i].begin(), strong_links[i].end(), std::back_inserter(conclusions));
            return true;
        }
    }
    return false;
}

bool LinkMap::all_values_equal(const std::vector<Link *> &v) const {
    if(v.empty())
        return false;

    int value = v.front()->get_value();
    for(std::vector<Link *>::const_iterator i = v.begin()+1; i != v.end(); ++i) {
        if((*i)->get_value() != value)
            return false;
    }
    return true;
}

void ForcingChainHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    for(Grid::iterator i = grid.begin(); i != grid.end(); ++i) { 
        Cell &cell = *i;
        find_forcing_chain(cell, grid, consumer);
        if(!consumer.wants_more_hints()) {
            return;
        }
    }
}

void ForcingChainHintProducer::find_forcing_chain(Cell &cell, Grid &grid, HintConsumer &consumer) const
{
    std::vector<Link *> links;
    LinkMap allLinks;

    for(int value = 1; value < 10; ++value) {
        if(cell.has_choice(value)) {
            StrongLink *link = new StrongLink(0, cell.get_idx(), value);
            LinkMap linkMap;
            Grid backup(grid);
            if(find_contradiction(link, linkMap, backup, grid, consumer)) {
                std::for_each(links.begin(), links.end(), destroy<Link *>());
                return;
            }
            links.push_back(link);
            allLinks.insert_all(linkMap);    
        }
    }

    if(!find_common_conclusion(allLinks, links.size(), grid, consumer)) {
        std::for_each(links.begin(), links.end(), destroy<Link *>());
    }
}

bool ForcingChainHintProducer::find_contradiction(Link *link, 
                                                  LinkMap &linkMap, 
                                                  Grid &grid, 
                                                  Grid &original,                                                    
                                                  HintConsumer &consumer) const
{
    std::vector<Link *> links;
    Link *contradiction = linkMap.find_contradiction(link);

    if(contradiction) {
        consumer.consume_hint(new ForcingChainContradictionHint(original, link, contradiction));
        return true;
    }

    if(!linkMap.insert(link))
        return false;

    if(link->is_strong_link()) {
        find_weak_links(link, links, grid);
        Cell &cell = grid[link->get_cell_idx()];
        cell.set_value(link->get_value());
        grid.cleanup_choice(cell);
        find_links_with_one_choice_left(link, links, grid);
    }
    else {
        Cell &cell = grid[link->get_cell_idx()];
        cell.remove_choice(link->get_value());
        find_strong_links(link, links, grid);
    }

    for(std::vector<Link *>::iterator i = links.begin(); i != links.end(); ++i) {
        if(find_contradiction(*i, linkMap, grid, original, consumer))
            return true;
    }

    return false;
}

bool ForcingChainHintProducer::find_common_conclusion(LinkMap &allLinks, size_t nconclusions, Grid &grid, HintConsumer &consumer) const
{
    std::vector<Link *> conclusions;
    if(allLinks.find_common_conclusion(nconclusions, conclusions)) {
        consumer.consume_hint(new ForcingChainHint(grid, conclusions));
        return true;
    }
    return false;
}


inline int get_opposite_value(Cell &cell, int value) 
{
    for(int v = 1; v < 10; ++v) 
        if(cell.has_choice(v) && v != value)
            return v;
    return 0;
}

void ForcingChainHintProducer::find_strong_links(Link *link, std::vector<Link *> &links, Grid &grid) const
{
    Cell &cell = grid[link->get_cell_idx()];

    if(cell.get_num_choices() == 1) {
        links.push_back(new StrongLink(link, cell.get_idx(), get_opposite_value(cell, link->get_value())));
    }

    find_links_in_ranges(link, links, grid);
}


void ForcingChainHintProducer::find_links_with_one_choice_left(Link *link, std::vector<Link *> &links, Grid &grid) const
{
    RangeList::const_index_iterator begin =  RANGES.field_begin(link->get_cell_idx());
    RangeList::const_index_iterator end =  RANGES.field_end(link->get_cell_idx());

    for(RangeList::const_index_iterator i = begin; i != end; ++i) {
        Cell &cell = grid[*i];
        if(cell.get_num_choices() == 1) {
            links.push_back(new StrongLink(link, cell.get_idx(), cell.first_choice()));
        }            
    }
}

void ForcingChainHintProducer::fill_range_frequencies(const Range &range, Grid &grid, std::vector<std::vector<Cell *> > &frequencies) const 
{
    for(Range::const_iterator i = range.begin(); i != range.end(); ++i) {
        Cell &cell = grid[*i];

        for(int value = 1;  value < 10; ++value) {
            if(!cell.has_value() && cell.has_choice(value)) {
                frequencies[value].push_back(&cell);
            }
        }
    }
}

void ForcingChainHintProducer::find_links_in_ranges(Link *link, std::vector<Link *> &links, Grid &grid) const
{
    const std::vector<Range> &ranges = RANGES.get_field_ranges(link->get_cell_idx());
    std::vector<std::vector<Cell *> > frequencies(10);

    for(std::vector<Range>::const_iterator irange = ranges.begin(); irange != ranges.end(); ++irange) {
        fill_range_frequencies(*irange, grid, frequencies); 
        for(int value = 1; value < 10; ++value) {
            if(frequencies[value].size() == 1) {
                Cell *cell = frequencies[value].front();
                links.push_back(new StrongLink(link, cell->get_idx(), value));
            }
        }
    }
}

void ForcingChainHintProducer::find_weak_links(Link *link, std::vector<Link *> &links, Grid &grid) const
{
    RangeList::const_index_iterator begin =  RANGES.field_begin(link->get_cell_idx());
    RangeList::const_index_iterator end =  RANGES.field_end(link->get_cell_idx());

    for(RangeList::const_index_iterator i = begin; i != end; ++i) {
        Cell &cell = grid[*i];
        if(cell.has_choice(link->get_value()))
            links.push_back(new WeakLink(link, cell.get_idx(), link->get_value()));
    }
}
