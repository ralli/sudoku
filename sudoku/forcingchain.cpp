/*
 * Copyright (c) 2009, Ralph Juhnke
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following conditions
 * are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *    * Neither the name of "Ralph Juhnke" nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
#include "grid.hpp"
#include "hintconsumer.hpp"
#include "forcingchain.hpp"
#include "util.hpp"
#include "range.hpp"

#include "hiddendouble.hpp"
#include "nakeddouble.hpp"
#include "pointing.hpp"
#include "claiming.hpp"
#include "hiddentriple.hpp"
#include "swordfish.hpp"
#include "xwing.hpp"
#include "xywing.hpp"

class LinkFactory {
    std::vector<Link *> links;
public:
    LinkFactory();
    virtual ~LinkFactory();
    StrongLink *create_strong_link(Link *parent, int cell_idx, int value);
    WeakLink *create_weak_link(Link *parent, int cell_idx, int value);
private:
    LinkFactory(const LinkFactory &) {
    }
    LinkFactory &operator =(const LinkFactory &) {
        return *this;
    }
};

LinkFactory::LinkFactory() {

}

LinkFactory::~LinkFactory() {
    std::for_each(links.begin(), links.end(), destroy<Link *> ());
}

StrongLink *LinkFactory::create_strong_link(Link *parent, int cell_idx,
        int value) {
    StrongLink *link = new StrongLink(parent, cell_idx, value);
    links.push_back(link);
    return link;
}

WeakLink *LinkFactory::create_weak_link(Link *parent, int cell_idx, int value) {
    WeakLink *link = new WeakLink(parent, cell_idx, value);
    links.push_back(link);
    return link;
}

/*!
 * \brief builds a vector of a given forcing chain
 *
 * \param link the link, wich marks the end of the forcing chain
 * \param chain the vector to be filled
 */

inline void fill_chain(Link *link, std::vector<LinkEntry> &chain) {
    while (link) {
        chain.push_back(LinkEntry(link->is_strong_link(), link->get_cell_idx(), link->get_value()));
        link = link->get_parent();
    }

    std::reverse(chain.begin(), chain.end());
}

/*!
 * \brief prints the contents of a given link
 */
struct print_link_entry {
    const LinkEntry &link_entry;

    print_link_entry(const LinkEntry &link_entry) :
        link_entry(link_entry) {
    }

    void print(std::ostream &out) const {
        out << print_row_col(link_entry.get_cell_idx());
        if (link_entry.is_strong())
            out << "=";
        else
            out << "<>";
        out << link_entry.get_value();
    }
};

std::ostream &operator <<(std::ostream &out, const print_link_entry &pl) {
    pl.print(out);
    return out;
}


/*!
 * \brief prints a given forcing chain
 */
struct print_entry_chain {
    const std::vector<LinkEntry> &links;

    print_entry_chain(const std::vector<LinkEntry> &links) :
        links(links) {
    }

    void print(std::ostream &out) const {
        for (std::vector<LinkEntry>::const_iterator i = links.begin(); i
                != links.end(); ++i) {
            out << print_link_entry(*i);
            if (i + 1 != links.end()) {
                out << " => ";
            }
        }
    }
};

std::ostream &operator <<(std::ostream &out, const print_entry_chain &chain) {
    chain.print(out);
    return out;
}


/*!
 * \brief constructor
 */
ForcingChainHint::ForcingChainHint(Grid &grid, const std::vector<Link *> &links) :
    grid(grid), chains(links.size()) {
    for (size_t i = 0; i < links.size(); ++i) {
        fill_chain(links[i], chains[i]);
    }
}

/*!
 * \brief destructor
 */
ForcingChainHint::~ForcingChainHint() {

}

void ForcingChainHint::apply() {
    LinkEntry &link_entry = chains.front().back();
    Cell &cell = grid[link_entry.get_cell_idx()];

    if (link_entry.is_strong()) {
        cell.set_value(link_entry.get_value());
        grid.cleanup_choice(cell);
    } else {
        cell.remove_choice(link_entry.get_value());
    }
}

void ForcingChainHint::print_description(std::ostream &out) const {
    out << "forcing chain: conclusion: " << print_link_entry(chains.front().back());
    out << " start cell: " << print_row_col(
            chains.front().front().get_cell_idx());
    for (size_t i = 0; i < chains.size(); ++i) {
        out << std::endl << "chain " << i + 1 << ": " << print_entry_chain(chains[i]);
    }
}

const char *ForcingChainHint::get_name() const {
    return "Forcing chain";
}

ForcingChainRangeHint::ForcingChainRangeHint(Grid &grid, const Range &range,
        std::vector<Link *> &conclusions) :
    grid(grid), range(range), chains(conclusions.size()) {
    for (size_t i = 0; i < conclusions.size(); ++i) {
        fill_chain(conclusions[i], chains[i]);
    }
}

void ForcingChainRangeHint::apply() {
    LinkEntry &link = chains.front().back();
    Cell &cell = grid[link.get_cell_idx()];

    if (link.is_strong()) {
        cell.set_value(link.get_value());
        grid.cleanup_choice(cell);
    } else {
        cell.remove_choice(link.get_value());
    }
}

void ForcingChainRangeHint::print_description(std::ostream &out) const {
    out << "range forcing chain: range: " << range.get_name()
            << " conclusion: " << print_link_entry(chains.front().back());
    out << " start cell: " << print_row_col(
            chains.front().front().get_cell_idx());
    for (size_t i = 0; i < chains.size(); ++i) {
        out << std::endl << "chain " << i + 1 << ": " << print_entry_chain(chains[i]);
    }
}

const char *ForcingChainRangeHint::get_name() const {
    return "Range forcing chain";
}

ForcingChainContradictionHint::ForcingChainContradictionHint(Grid &grid,
        Link *first_link, Link *second_link) :
    grid(grid) {
    fill_chain(first_link, first_chain);
    fill_chain(second_link, second_chain);
}

ForcingChainContradictionHint::~ForcingChainContradictionHint() {
}

void ForcingChainContradictionHint::apply() {
    LinkEntry &link = first_chain.front();
    Cell &cell = grid[link.get_cell_idx()];
    if (link.is_strong()) {
        cell.remove_choice(link.get_value());
    } else {
        cell.set_value(link.get_value());
        grid.cleanup_choice(cell);
    }
}

void ForcingChainContradictionHint::print_description(std::ostream &out) const {
    const LinkEntry &link = first_chain.front();
    if (link.is_strong()) {
        out << "contradiction: cell: " << print_row_col(link.get_cell_idx())
                << " cannot have value " << link.get_value() << std::endl;
    } else {
        out << "contradiction: cell: " << print_row_col(link.get_cell_idx())
                << " must have value " << link.get_value() << std::endl;
    }
    out << "first chain: " << print_entry_chain(first_chain) << std::endl;
    out << "second chain: " << print_entry_chain(second_chain);
}

const char *ForcingChainContradictionHint::get_name() const {
    return "Forcing chain contradiction";
}

Link::Link(Link *parent, int cell_idx, int value) :
    parent(parent), cell_idx(cell_idx), value(value) {
    if (parent)
        parent->children.push_back(this);
}

Link::~Link() {

}

const Link *Link::get_parent() const {
    return parent;
}

Link *Link::get_parent() {
    return parent;
}

int Link::get_cell_idx() const {
    return cell_idx;
}

int Link::get_value() const {
    return value;
}

Link::const_iterator Link::begin() const {
    return children.begin();
}

Link::const_iterator Link::end() const {
    return children.end();
}

Link *Link::get_head() {
    Link *p = this;
    while (p->get_parent() != 0) {
        p = p->get_parent();
    }
    return p;
}

StrongLink::StrongLink(Link *parent, int cell_idx, int value) :
    Link(parent, cell_idx, value) {
}

bool StrongLink::is_strong_link() const {
    return true;
}

WeakLink::WeakLink(Link *parent, int cell_idx, int value) :
    Link(parent, cell_idx, value) {
}

bool WeakLink::is_strong_link() const {
    return false;
}

LinkMap::LinkMap() :
    weak_links(81), strong_links(81) {
}

Link *LinkMap::find_contradiction(const Link *link) {
    if (link->is_strong_link())
        return find_strong_contradiction(link);
    else
        return find_weak_contradiction(link);
}

Link *LinkMap::find_strong_contradiction(const Link *link) {
    for (std::vector<Link *>::iterator i =
            strong_links[link->get_cell_idx()].begin(); i
            != strong_links[link->get_cell_idx()].end(); ++i) {
        if ((*i)->get_value() != link->get_value())
            return *i;
    }

    for (std::vector<Link *>::iterator i =
            weak_links[link->get_cell_idx()].begin(); i
            != weak_links[link->get_cell_idx()].end(); ++i) {
        if ((*i)->get_value() == link->get_value())
            return *i;
    }

    return 0;
}

Link *LinkMap::find_weak_contradiction(const Link *link) {
    std::vector<Link *> &links = strong_links[link->get_cell_idx()];

    for (std::vector<Link *>::iterator i = links.begin(); i != links.end(); ++i) {
        if ((*i)->get_value() == link->get_value())
            return *i;
    }

    return 0;
}

inline bool insert_into(std::vector<std::vector<Link *> > &linkmap, Link *link) {
    std::vector<Link *> &links = linkmap[link->get_cell_idx()];

    for (std::vector<Link *>::iterator i = links.begin(); i != links.end(); ++i) {
        if ((*i)->get_value() == link->get_value())
            return false;
    }
    links.push_back(link);
    return true;
}

bool LinkMap::insert(Link *link) {
    if (link->is_strong_link()) {
        return insert_into(strong_links, link);
    } else {
        return insert_into(weak_links, link);
    }
}

void LinkMap::insert_all(LinkMap &other) {
    for (int i = 0; i < 81; ++i) {
        std::copy(other.strong_links[i].begin(), other.strong_links[i].end(),
                std::back_inserter(strong_links[i]));
    }

    for (int i = 0; i < 81; ++i) {
        std::copy(other.weak_links[i].begin(), other.weak_links[i].end(),
                std::back_inserter(weak_links[i]));
    }
}

bool LinkMap::find_common_conclusion(size_t size,
        std::vector<Link *> &conclusions) {
    if (size == 0) {
        return false;
    }

    for (int i = 0; i < 81; ++i) {
        if (strong_links[i].size() == size && all_values_equal(strong_links[i])) {
            std::copy(strong_links[i].begin(), strong_links[i].end(),
                    std::back_inserter(conclusions));
            return true;
        }
    }
    for (int i = 0; i < 81; ++i) {
        size_t frequencies[10];
        std::vector<Link *> &links = weak_links[i];
        build_frequencies(links, frequencies);
        for (int value = 1; value < 10; ++value) {
            if (frequencies[value] == size) {
                for (std::vector<Link *>::iterator j = links.begin(); j
                        != links.end(); ++j) {
                    if ((*j)->get_value() == value) {
                        conclusions.push_back(*j);
                    }
                }
                return true;
            }
        }
    }
    return false;
}

void LinkMap::build_frequencies(const std::vector<Link *> &links,
        size_t frequencies[10]) const {

    std::fill(frequencies, frequencies + 10, 0);
    for (std::vector<Link *>::const_iterator i = links.begin(); i
            != links.end(); ++i) {
        ++frequencies[(*i)->get_value()];
    }
}

bool LinkMap::all_values_equal(const std::vector<Link *> &v) const {
    if (v.empty())
        return false;

    int value = v.front()->get_value();
    for (std::vector<Link *>::const_iterator i = v.begin() + 1; i != v.end(); ++i) {
        if ((*i)->get_value() != value)
            return false;
    }
    return true;
}

inline Link *LinkMap::find_weak_link(const Link *link) {
    std::vector<Link *> &links = weak_links[link->get_cell_idx()];
    for (std::vector<Link *>::iterator j = links.begin(); j != links.end(); ++j) {
        if ((*j)->get_value() == link->get_value())
            return *j;
    }
    return 0;
}

inline Link *LinkMap::find_strong_link(const Link *link) {
    std::vector<Link *> &links = strong_links[link->get_cell_idx()];
    for (std::vector<Link *>::iterator j = links.begin(); j != links.end(); ++j) {
        if ((*j)->get_value() == link->get_value())
            return *j;
    }
    return 0;
}

bool LinkMap::find_conclusion(LinkMap &link_map,
        std::vector<Link *> &links_found) {
    for (int i = 0; i < 81; ++i) {
        std::vector<Link *> &links = strong_links[i];
        for (std::vector<Link *>::iterator j = links.begin(); j != links.end(); ++j) {
            Link *link = link_map.find_strong_link(*j);
            if (link != 0) {
                links_found.push_back(*j);
                links_found.push_back(link);
                return true;
            }
        }
    }

    for (int i = 0; i < 81; ++i) {
        std::vector<Link *> &links = weak_links[i];
        for (std::vector<Link *>::iterator j = links.begin(); j != links.end(); ++j) {
            Link *link = link_map.find_weak_link(*j);
            if (link != 0) {
                links_found.push_back(*j);
                links_found.push_back(link);
                return true;
            }
        }
    }

    return false;
}

void LinkMap::insert_unique_children(Link *link) {
    std::queue<Link *> q;
    insert(link);
    for (Link::const_iterator i = link->begin(); i != link->end(); ++i) {
        q.push(*i);
    }
    while (!q.empty()) {
        Link *l = q.front();
        q.pop();
        insert(l);
        for (Link::const_iterator i = l->begin(); i != l->end(); ++i) {
            q.push(*i);
        }
    }
}

struct QueueStrategy {
    std::queue<Link *> queue;

    void push(Link *link) {
        queue.push(link);
    }

    Link *pop() {
        Link *link = queue.front();
        queue.pop();
        return link;
    }

    bool empty() const {
        return queue.empty();
    }
};

void ForcingChainHintProducer::find_hints(Grid &grid, HintConsumer &consumer) {
    for (int value = 1; value < 10; ++value) {
        std::vector<Link *> queue_links;
        LinkFactory factory;

        find_forcing_chain<QueueStrategy> (value, grid, consumer, queue_links,
                factory, 0);
        if (!consumer.wants_more_hints()) {
            return;
        }

        analyze_links(queue_links, value, grid, consumer);
        if (!consumer.wants_more_hints()) {
            return;
        }
    }

    for (int value = 1; value < 10; ++value) {
        std::vector<HintProducer *> hint_producers;

        hint_producers.push_back(new NakedDoubleHintProducer());
        hint_producers.push_back(new HiddenDoubleHintProducer());
        hint_producers.push_back(new PointingHintProducer());
        hint_producers.push_back(new ClaimingHintProducer());
        hint_producers.push_back(new HiddenTripleHintProducer());
        hint_producers.push_back(new SwordfishHintProducer());
        hint_producers.push_back(new XWingHintProducer());
        hint_producers.push_back(new XYWingHintProducer());

        for (std::vector<HintProducer *>::iterator i = hint_producers.begin(); i
                != hint_producers.end(); ++i) {
            std::vector<Link *> queue_links;
            LinkFactory factory;

            find_forcing_chain<QueueStrategy> (value, grid, consumer,
                    queue_links, factory, *i);
            if (!consumer.wants_more_hints()) {
                return;
            }
            analyze_links(queue_links, value, grid, consumer);
            if (!consumer.wants_more_hints()) {
                return;
            }
        }

        std::for_each(hint_producers.begin(), hint_producers.end(), destroy<
                HintProducer *> ());
    }
}

void ForcingChainHintProducer::analyze_links(std::vector<Link *> &links,
        int value, Grid &grid, HintConsumer &consumer) const {
    std::vector<std::vector<Link *> > cell_links(81);

    for (std::vector<Link *>::iterator x = links.begin(); x != links.end(); ++x) {
        Link *p = *x;
        if (p->is_strong_link())
            cell_links[p->get_cell_idx()].push_back(p);
    }

    for (RangeList::const_iterator irange = RANGES.begin(); irange
            != RANGES.end(); ++irange) {
        const Range &range = *irange;
        LinkMap all_links;
        size_t count = 0;

        for (Range::const_iterator i = range.begin(); i != range.end(); ++i) {
            std::vector<Link *> &range_links = cell_links[*i];
            for (std::vector<Link *>::const_iterator j = range_links.begin(); j
                    != range_links.end(); ++j) {
                Link *l = *j;
                LinkMap link_map;
                link_map.insert_unique_children(l);
                ++count;
                all_links.insert_all(link_map);
            }
        }

        std::vector<Link *> conclusions;
        if (all_links.find_common_conclusion(count, conclusions)) {
#if 0
            Link *p = conclusions.front()->get_head();
            std::vector<Link *> &bla = cell_links[p->get_cell_idx()];
            std::cout << "xxx size: " << count << " size2: "
            << cell_links[p->get_cell_idx()].size();
            for (std::vector<Link *>::const_iterator i = bla.begin(); i
                    != bla.end(); ++i)
            std::cout << ' ' << print_link(*i);
            std::cout << std::endl;
            std::cout << "links: ";
            for (std::vector<Link *>::const_iterator i = links.begin(); i
                    != links.end(); ++i)
            std::cout << ' ' << print_link(*i);
            std::cout << std::endl;
#endif
            ForcingChainRangeHint *hint = new ForcingChainRangeHint(grid,
                    range, conclusions);
            if (!consumer.consume_hint(hint))
                return;
        }
    }
}

/*!
 * generates all forcing chains starting from a certain starting cell.
 * tries to find contradictions. uses different strategies (bfs-search or dfs-search)
 * since a bread first seach tends to find shorter chains and the dfs tends to find more
 * contradictions. (this is just an obervation. i have not been trying to prove it).
 */

template<class Strategy>
void ForcingChainHintProducer::find_forcing_chain(int value, Grid &grid,
        HintConsumer &consumer, std::vector<Link *> &links,
        LinkFactory &factory, HintProducer *producer) const {
    LinkMap allLinks;
    int link_count = 0;
    std::vector<Link *> local_links;

    for (Grid::iterator i = grid.begin(); i != grid.end(); ++i) {
        Cell &cell = *i;
        if (cell.has_choice(value)) {
            ++link_count;
            Grid weak_backup(grid);
            WeakLink *weak_link = factory.create_weak_link(0, cell.get_idx(),
                    value);
            LinkMap weak_link_map;

            if (find_contradiction<Strategy> (weak_link, weak_link_map,
                    weak_backup, grid, consumer, factory, producer)) {
                return;
            }

            StrongLink *strong_link = factory.create_strong_link(0,
                    cell.get_idx(), value);
            LinkMap strong_link_map;
            Grid strong_backup(grid);
            if (find_contradiction<Strategy> (strong_link, strong_link_map,
                    strong_backup, grid, consumer, factory, producer)) {
                return;
            }

            if (find_conclusion(weak_link, strong_link, weak_link_map,
                    strong_link_map, grid, consumer)) {
                return;
            }

            local_links.push_back(weak_link);
            local_links.push_back(strong_link);
            allLinks.insert_all(strong_link_map);
        }
    }

    if (!find_common_conclusion(allLinks, link_count, grid, consumer)) {
        std::copy(local_links.begin(), local_links.end(), std::back_inserter(
                links));
    }
}

bool ForcingChainHintProducer::find_conclusion(Link *weak_link,
        Link *strong_link, LinkMap &weak_link_map, LinkMap &strong_link_map,
        Grid &grid, HintConsumer &consumer) const {
    std::vector<Link *> links;
    if (!weak_link_map.find_conclusion(strong_link_map, links))
        return false;
    ForcingChainHint *hint = new ForcingChainHint(grid, links);
    consumer.consume_hint(hint);
    return true;
}

/*!
 * \brief given a starting assumption, this method tries to find a contradiction,
 * i.e. two conclusions which cannot be true at the same time.
 *
 */
template<class Strategy>
bool ForcingChainHintProducer::find_contradiction(Link *start,
        LinkMap &linkMap, Grid &grid, Grid &original, HintConsumer &consumer,
        LinkFactory &factory, HintProducer *producer) const {
    Strategy qstrong;
    Strategy qweak;

    if (start->is_strong_link())
        qstrong.push(start);
    else
        qweak.push(start);

    while (!(qstrong.empty() && qweak.empty())) {
        Link * link;

        if (!qstrong.empty())
            link = qstrong.pop();
        else
            link = qweak.pop();

        std::vector<Link *> links;
        Link *contradiction = linkMap.find_contradiction(link);

        if (contradiction) {
            consumer.consume_hint(new ForcingChainContradictionHint(original,
                    link, contradiction));
            return true;
        }

        /*
         * a return value of false means, the link has already been
         * found. since we will come to the same conclusions again,
         * I will cut the search here.
         */
        if (!linkMap.insert(link))
            continue;

        if (link->is_strong_link()) {
            find_weak_links(link, links, grid, factory);
            Cell &cell = grid[link->get_cell_idx()];
            cell.set_value(link->get_value());
            cell.clear_choices();
            grid.cleanup_choice(cell);
            find_links_in_ranges(link, links, grid, factory);
            find_links_with_one_choice_left(link, links, grid, factory);
        } else {
            Cell &cell = grid[link->get_cell_idx()];
            cell.remove_choice(link->get_value());
            find_strong_links(link, links, grid, factory);
        }

        for (std::vector<Link *>::iterator i = links.begin(); i != links.end(); ++i) {
            if ((*i)->is_strong_link())
                qstrong.push(*i);
            else
                qweak.push(*i);
        }

        if (qstrong.empty() && qweak.empty() && producer) {
            find_advanced_links(link, links, grid, factory, producer);
            for (std::vector<Link *>::iterator i = links.begin(); i
                    != links.end(); ++i) {
                if ((*i)->is_strong_link())
                    qstrong.push(*i);
                else
                    qweak.push(*i);
            }
        }
    }
    return false;
}

bool ForcingChainHintProducer::find_common_conclusion(LinkMap &allLinks,
        size_t nconclusions, Grid &grid, HintConsumer &consumer) const {
    if (nconclusions == 0)
        return false;
    std::vector<Link *> conclusions;
    if (allLinks.find_common_conclusion(nconclusions, conclusions)) {
        consumer.consume_hint(new ForcingChainHint(grid, conclusions));
        return true;
    }
    return false;
}

inline int get_opposite_value(Cell &cell, int value) {
    for (int v = 1; v < 10; ++v)
        if (cell.has_choice(v) && v != value)
            return v;
    return 0;
}

void ForcingChainHintProducer::find_strong_links(Link *link,
        std::vector<Link *> &links, Grid &grid, LinkFactory &factory) const {
    Cell &cell = grid[link->get_cell_idx()];

    if (cell.get_num_choices() == 1) {
        links.push_back(factory.create_strong_link(link, cell.get_idx(),
                get_opposite_value(cell, link->get_value())));
    }

    find_links_in_ranges(link, links, grid, factory);
}

void ForcingChainHintProducer::find_links_with_one_choice_left(Link *link,
        std::vector<Link *> &links, Grid &grid, LinkFactory &factory) const {
    RangeList::const_index_iterator begin = RANGES.field_begin(
            link->get_cell_idx());
    RangeList::const_index_iterator end =
            RANGES.field_end(link->get_cell_idx());

    for (RangeList::const_index_iterator i = begin; i != end; ++i) {
        Cell &cell = grid[*i];
        if (cell.get_num_choices() == 1) {
            links.push_back(factory.create_strong_link(link, cell.get_idx(),
                    cell.first_choice()));
        }
    }
}

void ForcingChainHintProducer::fill_range_frequencies(const Range &range,
        Grid &grid, std::vector<std::vector<Cell *> > &frequencies) const {
    for (Range::const_iterator i = range.begin(); i != range.end(); ++i) {
        Cell &cell = grid[*i];
        for (int value = 1; value < 10; ++value) {
            if (cell.has_choice(value)) {
                frequencies[value].push_back(&cell);
            }
        }
    }
}

void ForcingChainHintProducer::find_links_in_ranges(Link *link, std::vector<
        Link *> &links, Grid &grid, LinkFactory &factory) const {
    const std::vector<Range> &ranges = RANGES.get_field_ranges(
            link->get_cell_idx());

    for (std::vector<Range>::const_iterator irange = ranges.begin(); irange
            != ranges.end(); ++irange) {
        std::vector<std::vector<Cell *> > frequencies(10);

        fill_range_frequencies(*irange, grid, frequencies);
        for (int value = 1; value < 10; ++value) {
            if (frequencies[value].size() == 1) {
                Cell *cell = frequencies[value].front();
                links.push_back(factory.create_strong_link(link,
                        cell->get_idx(), value));
            }
        }
    }
}

void ForcingChainHintProducer::find_weak_links(Link *link,
        std::vector<Link *> &links, Grid &grid, LinkFactory &factory) const {
    RangeList::const_index_iterator begin = RANGES.field_begin(
            link->get_cell_idx());
    RangeList::const_index_iterator end =
            RANGES.field_end(link->get_cell_idx());
    Cell &c = grid[link->get_cell_idx()];

    for (int value = 1; value < 10; ++value) {
        if (value != link->get_value() && c.has_choice(value)) {
            links.push_back(factory.create_weak_link(link, c.get_idx(), value));
        }
    }

    for (RangeList::const_index_iterator i = begin; i != end; ++i) {
        Cell &cell = grid[*i];
        if (cell.has_choice(link->get_value()))
            links.push_back(factory.create_weak_link(link, cell.get_idx(),
                    link->get_value()));
    }
}

class LinkCreatingHintConsumer: public HintConsumer {
    Link *parent;
    std::vector<Link *> links;
    LinkFactory &factory;
public:
    LinkCreatingHintConsumer(Link *parent, LinkFactory &factory) :
        parent(parent), links(), factory(factory) {

    }

    bool consume_hint(Hint *hint) {
        IndirectHint *h = dynamic_cast<IndirectHint *> (hint);
        std::vector<std::pair<Cell *, int> >::const_iterator begin =
                h->get_choices_to_remove().begin();
        std::vector<std::pair<Cell *, int> >::const_iterator end =
                h->get_choices_to_remove().end();

        for (std::vector<std::pair<Cell *, int> >::const_iterator i = begin; i
                != end; ++i) {
            links.push_back(factory.create_weak_link(parent,
                    i->first->get_idx(), i->second));
        }

        delete hint;

        return false;
    }

    bool wants_more_hints() const {
        return links.empty();
    }

    std::vector<Link *>::iterator begin() {
        return links.begin();
    }
    std::vector<Link *>::iterator end() {
        return links.end();
    }
};

bool ForcingChainHintProducer::find_advanced_links(Link *parent, std::vector<
        Link *> &links, Grid &grid, LinkFactory &factory,
        HintProducer *producer) const {

    LinkCreatingHintConsumer consumer(parent, factory);
    producer->find_hints(grid, consumer);
    std::copy(consumer.begin(), consumer.end(), std::back_inserter(links));
    return !consumer.wants_more_hints();
}
