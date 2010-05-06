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

#ifndef GRID_HPP_
#define GRID_HPP_

#include <vector>
#include <iosfwd>

/*!
 * \brief
 * maintains a list of choices (possible values) for a given cell.
 */
class Choices {
    bool choices[10];
    int num_choices;
public:
    /*!
     * \brief
     * constructor
     */
    Choices();

    /*!
     * \brief
     * copy constructor
     */
    Choices(const Choices &others);

    /*!
     * \brief
     * assignment operator
     * \return this
     */
    Choices & operator =(const Choices &others);

    /*!
     * \brief returns the number of choices left
     * returns the number of choices left
     */
    int get_num_choices() const;

    /*!
     * \brief
     * returns true, if the value given is a valid choice
     * \param value the value to be tested
     * \return true, if the value given is a valid choice
     */
    bool has_choice(int value) const;

    /*!
     * \brief adds a choice to the list of valid choices
     * \param value the value to be added
     */
    void add_choice(int value);


    /**
     * removes a choice to the list of valid choices.
     *
     * @param value the value to be added
     */
    void remove_choice(int value);

    /**
     * removes all choices from the list of choices.
     */
    void clear();

    /**
     * adds all choices from the list of choices.
     */
    void set_all();

    /**
     * returns the first valid choice or '0' if there is no choice left.
     */
    int first_choice() const;
};

/**
 * a single cell of a sudoku puzzle
 */
class Cell {
    int idx;
    int value;
    Choices choices;
public:
    /**
     * Constructor
     *
     * @param the cells index
     */
    Cell(int idx);

    /**
     * copy constructor
     *
     * @param other the cell to be copied
     */
    Cell(const Cell &other);

    /**
     * assignment operator
     */
    Cell & operator =(const Cell &other);

    /**
     * the cells value
     *
     * @return the cells value
     */
    int get_value() const;

    /**
     * the cells value
     *
     * param value the cells value
     */
    void set_value(int value);

    /**
     * returns true, if the cell has a value
     *
     * @return true, if the cell has a value
     */
    bool has_value() const;

    /**
     * returns the cells row in the range 0..8
     *
     * @return the cells row in the range 0..8
     */
    int get_row() const;

    /**
     * returns the cells column in the range 0..8
     *
     * @return the cells column in the range 0..8
     */
    int get_col() const;

    /**
     * returns the index of the block (house) of the cell in the
     * range 0..8.
     * the indexes are given as follows:
     * 
        \verbatim
     0 1 2
     3 4 5
     6 7 8 \endverbatim
     *
     *  @return the cells block index
     */
    int get_block_idx() const;

    /**
     * returns the cells index in the range 0..80.
     * the index in the top left cell is 0 and the one 
     * in the bottom right is 80.
     *
     * @return the cells index
     */
    int get_idx() const;

    /**
     * returns true, if a value is a valid choice for this cell.
     *
     * @param value the value to be checked
     * 
     * @return true, if a value is a valid choice for this cell.
     */
    bool has_choice(int value) const;

    /**
     * adds a choice to the list of valid choices for this cell.
     *
     * @param value the choice to be added
     */
    void add_choice(int value);

    /**
     * removes a choice to the list of valid choices for this cell.
     *
     * @param value the choice to be removed
     */
    void remove_choice(int value);

    /**
     * clears the list of valid choices for this cell.
     */
    void clear_choices();

    /**
     * returns the number of valid choices for this cell.
     *
     * @return the number of valid choices for this cell.
     */
    int get_num_choices() const;

    /**
     * prints the cells choices
     *
     * @param out the stream to print on
     */
    void print_choices(std::ostream &out) const;

    /**
     * returns the first valid choice of this cell or zero
     * if there is no valid choice left.
     *
     * @return the first valid choice or zero
     */
    int first_choice() const;

    /**
     * returns the list of all valid choices.
     *
     * @return the list of all valid choices.
     */
    const Choices &get_choices() const;

    /**
     * sets all choices to be a valid choice for this cell.
     */
    void set_all_choices();
};

class Grid {
public:
    typedef std::vector<Cell>::iterator iterator;
    typedef std::vector<Cell>::const_iterator const_iterator;
private:
    std::vector<Cell> cells;
public:
    /**
     * Constructor
     */
    Grid();

    /**
     * copy constructor
     *
     * @param other the grid to be copied
     */
    Grid(const Grid &other);

    /**
     * assignment operator
     *
     * @param other the grid to assign to *this
     */
    Grid & operator =(const Grid &other);

    /**
     * initializes all cells
     *
     * initializing means clearing the grid.
     * After calling this method all cells
     * do not contain a value and all choices are valid
     * for each cell.
     */
    void init_cells();

    /**
     * loads a grid from an input stream.
     *
     * the grids contents are just a sequence of 89 characters on a single line.
     * each digit (1..9) is interpreted as a cells content. if the character 
     * read is not a digit in the range 1..9 then the cell is considered empty.
     * 
     * @param in the stream to load the cells from
     */
    void load(std::istream &in);

    /**
     * prints a grid in a row / column format.
     *
     * @param out the stream to print on
     */
    void print(std::ostream &out) const;

    /**
     * prints a grid on a single line in the format
     * described in the method Grid::load.
     *
     * the grid is *not* terminated by a newline character.
     *
     * @param out the stream to print on
     */

    void println(std::ostream &out) const;

    /**
     * prints a grid in a row / column format.
     *
     * @param out the stream to print on
     */
    void print_choices(std::ostream &out) const;

    /**
     * prints the grids status.
     *
     * prints information about the number of choices left to solve and the
     * number of cells left to solve.
     *
     * @param out the stream to print on
     */
    void print_status(std::ostream &out) const;

    /**
     * returns the number of cells left to solve.
     *
     * @return the number of cells left to solve.
     */
    int get_to_do() const;

    /**
     * returns the number of choices left to solve.
     *
     * @param returns the number of choices left to solve.
     */
    int get_num_choices() const;

    /**
     * initializes the lists of valid choices for each
     * cell on the grid.
     *
     * see cleanup_choice for details.
     */
    void cleanup_choices();

    /**
     * if the cell has a value, i.e. it is already solved.
     * this value is removed from the list of choices of all
     * adjacent cells. the list of valid choices for the cell
     * is cleared, since there are no valid choices left for
     * this cell.
     *
     * @param cell the cell
     *
     */
    void cleanup_choice(Cell &cell);

    /**
     * removes all invalid choices from a given cell.
     *
     * iterates over all adjacent cells. if a adjacent cell has a
     * value, then this value is removed from the list of valid choices.
     *
     * @param cell the cell to remove the invalid choices from.
     */
    void remove_invalid_cell_choices(Cell &cell);

    /**
     * clears a cells value and sets the valid
     * choices of all adjacent cells.
     */
    void clear_cell_value(Cell &cell);

    /**
     * returns an iterator pointing to the first cell of the grid.
     *
     * @return an iterator pointing to the first cell of the grid.
     */
    iterator begin();

    /**
     * returns an iterator pointing past the last cell of the grid.
     *
     * @return an iterator pointing past the last cell of the grid.
     */
    iterator end();

    /**
     * returns an iterator pointing to the first cell of the grid.
     *
     * @return an iterator pointing to the first cell of the grid.
     */
    const_iterator begin() const;

    /**
     * returns an iterator pointing past the last cell of the grid.
     *
     * @return an iterator pointing past the last cell of the grid.
     */
    const_iterator end() const;

    /**
     * returns a grids cell.
     *
     * @param the index in the range 0..81 of the cell.
     * 
     * @return the cell of the grid
     */
    Cell & operator[](int idx);

    /**
     * returns a grids cell.
     *
     * @param the index in the range 0..81 of the cell.
     *
     * @return the cell of the grid
     */
    const Cell & operator[](int idx) const;
};

inline Choices::Choices() :
num_choices(9) {
    std::fill(choices, choices + 10, true);
}

inline Choices::Choices(const Choices &other) :
num_choices(other.num_choices) {
    std::copy(other.choices, other.choices + 10, choices);
}

inline Choices &Choices::operator =(const Choices &other) {
    if (this == &other)
        return *this;

    std::copy(other.choices, other.choices + 10, choices);
    num_choices = other.num_choices;

    return *this;
}

inline int Choices::get_num_choices() const {
    return num_choices;
}

inline bool Choices::has_choice(int value) const {
    return choices[value];
}

inline void Choices::add_choice(int value) {
    if (choices[value] == false) {
        choices[value] = true;
        ++num_choices;
    }
}

inline void Choices::remove_choice(int value) {
    if (choices[value] == true) {
        choices[value] = false;
        --num_choices;
    }
}

inline void Choices::clear() {
    std::fill(choices, choices + 10, false);
    num_choices = 0;
}

inline void Choices::set_all() {
    std::fill(choices, choices + 10, true);
    num_choices = 9;
}

inline int Choices::first_choice() const {
    if (num_choices == 0)
        return 0;

    for (int i = 1; i < 10; ++i) {
        if (choices[i])
            return i;
    }

    return 0;
}

inline Cell::Cell(int idx) :
idx(idx), value(0), choices() {
}

inline Cell::Cell(const Cell &other) :
idx(other.idx), value(other.value), choices(other.choices) {
}

inline Cell &Cell::operator =(const Cell &other) {
    if (this == &other)
        return *this;

    idx = other.idx;
    value = other.value;
    choices = other.choices;

    return *this;
}

inline int Cell::get_value() const {
    return value;
}

inline bool Cell::has_value() const {
    return value > 0;
}

inline void Cell::set_value(int value) {
    this->value = value;
}

inline int Cell::get_row() const {
    return idx / 9;
}

inline int Cell::get_col() const {
    return idx % 9;
}

inline int Cell::get_block_idx() const {
    return get_row() / 3 * 3 + get_col() / 3;
}

inline int Cell::get_idx() const {
    return idx;
}

inline bool Cell::has_choice(int value) const {
    return choices.has_choice(value);
}

inline void Cell::add_choice(int value) {
    choices.add_choice(value);
}

inline void Cell::remove_choice(int value) {
    choices.remove_choice(value);
}

inline void Cell::clear_choices() {
    choices.clear();
}

inline int Cell::get_num_choices() const {
    return choices.get_num_choices();
}

inline int Cell::first_choice() const {
    return choices.first_choice();
}

inline const Choices &Cell::get_choices() const {
    return choices;
}

inline void Cell::set_all_choices() {
    choices.set_all();
}

inline Grid::Grid() {
    init_cells();
}

inline Grid::Grid(const Grid &other) {
    cells = other.cells;
}

inline Grid &Grid::operator =(const Grid &other) {
    if (this == &other)
        return *this;

    cells = other.cells;

    return *this;
}

inline void Grid::init_cells() {
    cells.clear();
    for (int i = 0; i < 81; ++i) {
        cells.push_back(Cell(i));
    }
}

inline int Grid::get_to_do() const {
    int todo = 0;

    for (int i = 0; i < 81; ++i)
        if (!cells[i].has_value())
            ++todo;

    return todo;
}

inline int Grid::get_num_choices() const {
    int result = 0;
    for (int i = 0; i < 81; ++i)
        result += cells[i].get_num_choices();
    return result;
}

inline Grid::iterator Grid::begin() {
    return cells.begin();
}

inline Grid::iterator Grid::end() {
    return cells.end();
}

inline Grid::const_iterator Grid::begin() const {
    return cells.begin();
}

inline Grid::const_iterator Grid::end() const {
    return cells.end();
}

inline Cell &Grid::operator[](int idx) {
    return cells[idx];
}

inline const Cell & Grid::operator[](int idx) const {
    return cells[idx];
}

inline void Grid::cleanup_choices() {
    for (iterator i = begin(); i != end(); ++i) {
        cleanup_choice(*i);
    }
}

#endif
