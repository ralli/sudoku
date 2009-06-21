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

#ifndef FORCINGCHAIN_HPP
#define FORCINGCHAIN_HPP

#include <vector>
#include "hint.hpp"

class Range;
class Link;
class LinkFactory;

/*!
 * \brief a forcing chain hint: for a given cell all choices come to the same
 * conclusion.
 * This conclusion may be either a strong link (a cell must have a given value)
 * or a weak link (a cell cannot have a given value).
 */

class ForcingChainHint: public Hint {
    /*! \brief the grid the hint will be applied to */
    Grid &grid;
    /*! \brief the ends of the conclusion chains */
    std::vector<Link *> links;
    /*! \brief the forcing chains (which all end in the same links) */
    std::vector<std::vector<Link *> > chains;
public:
    /*!
     * \brief constructor
     * \param grid the Grid, this hint will be applied to
     * \param links the list of the ends of the forcing chains.
     */
    ForcingChainHint(Grid &grid, const std::vector<Link *> &links);
    /*!
     * \brief destructor
     */
    virtual ~ForcingChainHint();
    void apply();
    void print_description(std::ostream &out) const;
};

class ForcingChainRangeHint: public Hint {
    Grid &grid;
    const Range &range;
    std::vector<Link *> &conclusions;
    std::vector<std::vector<Link *> > chains;
public:
    ForcingChainRangeHint(Grid &grid, const Range &range,
            std::vector<Link *> &conclusions);
    void apply();
    void print_description(std::ostream &out) const;
};

/*!
 * \brief a forcing chain contradiction.
 * This means, two branches of the same forcing chain come to different
 * conclusions which contradict each other. Ex. cell(1,1) = 1 and cell(1,1) <> 1.
 * This means, the start of the forcing chain may be eliminated.
 */
class ForcingChainContradictionHint: public Hint {
    Grid &grid;
    Link *first_link;
    Link *second_link;
    std::vector<Link *> first_chain;
    std::vector<Link *> second_chain;
public:
    /*!
     * \brief constructor
     * \param grid the grid this hint will be applied to
     * \param first_link the end of the first branch of the forcing chain
     * \param second_link the end of the second branch of the forcing chain
     */
    ForcingChainContradictionHint(Grid &grid, Link *first_link,
            Link *second_link);
    /*!
     * \brief destructor
     */
    virtual ~ForcingChainContradictionHint();
    void apply();
    void print_description(std::ostream &out) const;
};

/*!
 * \brief a link is the list of conclusions resulting from a single assumption.
 *
 *  there are two types of links:
 *      * a strong link means: if the parent assumption is true, then the cell must have a certain value.
 *      * a weak link means: if the parent assumption is true, then the cell cannot have a certain value.
 */

class Link {
public:
    /*! \brief iterator over the children of a link */
    typedef std::vector<Link *>::iterator iterator;
    /*! \brief const_iterator over the children of a link */
    typedef std::vector<Link *>::const_iterator const_iterator;
private:
    /*!
     * \brief pointer to the parent assumption (or null,
     * if there is no such assumption)
     */
    Link *parent;

    /*!
     * \brief index of the cell, this link refers to
     */
    int cell_idx;

    /*!
     * \brief value which the cell must have (in case of a strong link)
     * or cannot have (in case of a weak link)
     * */
    int value;

    /*!
     * \brief the conclusions which result of this link
     */
    std::vector<Link *> children;
public:
    /*!
     * \brief constructor
     * \param parent the parent assumption
     * \param  index of the cell, this link refers to
     * \param value value which the cell must have (in case of a strong link)
     *        or cannot have (in case of a weak link
     */
    Link(Link *parent, int cell_idx, int value);

    /*!
     * \brief destructor
     */
    virtual ~Link();

    /*!
     * \brief returns the parent assumption of this link
     * \return the parent assumption of this link
     */
    const Link *get_parent() const;

    /*!
     * \brief returns the parent assumption of this link
     * \return the parent assumption of this link
     */
    Link *get_parent();

    /*!
     * \brief returns the index of the cell, this link refers to
     * \return the index of the cell, this link refers to
     */
    int get_cell_idx() const;

    /*!
     * \brief returns value value which the cell must have (in case of a strong link)
     *        or cannot have (in case of a weak link)
     *
     * \return value value which the cell must have (in case of a strong link)
     *        or cannot have (in case of a weak link)
     */
    int get_value() const;

    /*!
     * \brief returns an iterator to the first child of this link
     * \ŗeturn iterator to the first child of this link
     */
    const_iterator begin() const;

    /*!
     * \brief returns an iterator past the last child of this link
     * \ŗeturn iterator past the last child of this link
     */
    const_iterator end() const;

    /*!
     * \brief returns true, if this link is a strong link and false,
     * if this link is a weak link
     *
     * \return  true, if this link is a strong link and false,
     * if this link is a weak link
     */
    virtual bool is_strong_link() const = 0;

    Link *get_head();
private:
    Link(const Link *other) {
    }
    Link &operator =(const Link *other) {
        return *this;
    }
};

/*!
 * \brief a strong link states, that a cell must have a given value if
 * its parent assumption is true.
 */
class StrongLink: public Link {
public:
    StrongLink(Link *parent, int cell_idx, int value);
    virtual bool is_strong_link() const;
};

/*!
 * \brief a weak link states, that a cell cannot have a given value if
 * its parent assumption is true.
 */
class WeakLink: public Link {
public:
    WeakLink(Link *parent, int cell_idx, int value);
    virtual bool is_strong_link() const;
};

/*!
 * \brief stores links.
 */
class LinkMap {
    std::vector<std::vector<Link *> > weak_links;
    std::vector<std::vector<Link *> > strong_links;
public:
    /*!
     * \brief constructor
     */
    LinkMap();

    /*!
     * \brief finds a constradiction to a given link
     * \param link the link to find the contradiction to
     * \return the link which is a contradiction or null, if no such link
     * has been found
     */
    Link *find_contradiction(const Link *link);

    /*!
     * \brief inserts a link into the map
     * \param link the link to be inserted into the map
     */
    bool insert(Link *link);

    /*!
     * \brief inserts a set of links into the map
     * \param other the links to be inserted
     */
    void insert_all(LinkMap &other);

    /*!
     *  \brief finds a list of common conclusions
     *  \param size the number of links that have to be found,
     *  to have a common conclusion
     *  \param the conclusion found
     *  \return true, if a conclusion has been found
     */
    bool find_common_conclusion(size_t size, std::vector<Link *> &conclusions);

    /*!
     * \brief
     *  checks all links strored in this map, if there is a matching link in the
     *  other map. if such a link is found returns a vector containing the matching
     *  links from both link maps.
     *
     *  \param link_map the other link map
     *  \return a vector either containing the two matching links or an empty vector.
     */
    bool find_conclusion(LinkMap &link_map, std::vector<Link *> &links_found);

    void insert_unique_children(Link *link);
private:
    LinkMap(const LinkMap &other) {
    }
    LinkMap &operator =(const LinkMap &other) {
        return *this;
    }
    Link *find_strong_contradiction(const Link *link);
    Link *find_weak_contradiction(const Link *link);
    /*!
     * \brief returns true, if all values in a given list are equal
     * \param v the list of links
     * \return true all links have equal values
     */
    bool all_values_equal(const std::vector<Link *> &v) const;
    /*!
     * \brief finds a matching weak link in the list of weak links
     * \param link the link to find
     * \return the link found or null if no matching link has been found
     */
    Link *find_weak_link(const Link *link);
    /*!
     * \brief finds a matching strong link in the list of strong links
     * \param link the link to find
     * \return the link found or null if no matching link has been found
     */
    Link *find_strong_link(const Link *link);
    /*!
     * \brief builds a histogram of all values in a given list of links
     * \param links the list of links to compute the histogram of
     * \param frequencies the histogram of values
     */
    void build_frequencies(const std::vector<Link *> &links,
            size_t frequencies[10]) const;

};

/*!
 * \brief a hint producer for forcing chains.
 * finds either contradictins (two branches of a forcing chain, which result in
 * conclusion negating each other) or common conclusions (all possible
 * values of a starting cell force another cell to have a certain value
 * or not to have a certain value).
 *
 * this implementation just uses singles (naked singles or hidden singles) to
 * produce strong or weak links. it would be cooler, if we could use the other
 * hint producers (doubles, x-wings etc.) produce these links...
 */
class ForcingChainHintProducer: public HintProducer {
public:
    void find_hints(Grid &grid, HintConsumer &consumer);
private:
    void analyze_links(std::vector<Link *> &links, int value, Grid &grid,
            HintConsumer &consumer) const;
    /*!
     * \brief tries finds to find a forcing chain (contradiction or
     * forcing chain) for a given cell.
     */
    template<class Strategy>
    void find_forcing_chain(int value, Grid &grid, HintConsumer &consumer,
            std::vector<Link *> &links, LinkFactory &factory,
            HintProducer *producer) const;

    /*!
     * \brief tries to find a contradiction to a given link recursively.
     * \param link the link to find the contradiction to
     * \param linkMap the links found so far, wich are candidates for a contradiction
     * \return true, if a contradiction has been found
     */
    template<class Strategy>
    bool find_contradiction(Link *start, LinkMap &linkMap, Grid &grid,
            Grid &original, HintConsumer &consumer, LinkFactory &factory,
            HintProducer *producer) const;

    /*!
     * \brief tries to find a common conclusion
     * \param allLinks the LinkMap to find the common conclusion in
     * \param nconclusions the numer of links the common conclusion must contain
     * \param grid the ForcingChainConclusionHint will be applied to
     * \param consumer the HintConsumer which will consume the ForcingChainConclusionHint
     * \return true, if a common conclusion has been found
     */
    bool find_common_conclusion(LinkMap &allLinks, size_t nconclusions,
            Grid &grid, HintConsumer &consumer) const;

    /*!
     * \brief finds all strong links resulting as conclusion from a given link.
     * \param link the link to find the strong links for
     * \param links the links found
     * \param grid the grid to find the strong links in
     */
    void find_strong_links(Link *link, std::vector<Link *> &links, Grid &grid,
            LinkFactory &factory) const;

    /*!
     * \brief finds all weak links resulting as conclusion from a given link.
     * \param link the link to find the links for
     * \param links the links found
     * \param grid the grid to find the links in
     */
    void find_weak_links(Link *link, std::vector<Link *> &links, Grid &grid,
            LinkFactory &factory) const;

    void find_links_with_one_choice_left(Link *link,
            std::vector<Link *> &links, Grid &grid, LinkFactory &factory) const;

    void find_links_in_ranges(Link *link, std::vector<Link *> &links,
            Grid &grid, LinkFactory &factory) const;

    bool find_advanced_links(Link *parent, std::vector<Link *> &links,
            Grid &grid, LinkFactory &factory, HintProducer *producer) const;

    void fill_range_frequencies(const Range &range, Grid &grid, std::vector<
            std::vector<Cell *> > &frequencies) const;

    /*!
     * \brief
     *    given a weak assumption (rncm <> x) and a strong assumption (rncm == x)
     *    and all conclusions from these assumptions. find a common conclusion.
     *    this means regardless of wich valus the cell rncm contains,
     *    this conclusion is true.
     */
    bool find_conclusion(Link *weak_link, Link *strong_link,
            LinkMap &weak_link_map, LinkMap &strong_link_map, Grid &grid,
            HintConsumer &consumer) const;
};

#endif
