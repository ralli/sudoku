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

#ifndef SUDOKUMODEL_HPP_
#define SUDOKUMODEL_HPP_

#include "gtkmm.h"
#include <string>
#include "../sudoku/grid.hpp"

#include "commands.hpp"

class Hint;

class SudokuModel: public Glib::Object {
public:
    /*! \brief a signal with no arguments returning void */
    typedef sigc::signal<void> type_signal_changed;
    /*! \brief the difficulty level of a sudoku */
    typedef enum {
        EASY, MEDIUM, HARD
    } DifficultyLevel;
private:
    int selected_cell;
    int highlighted_choice;
    type_signal_changed m_signal_changed;
    Grid grid;
    UndoManager undo_manager;
    DifficultyLevel difficulty_level;
    bool show_choices;
    Hint *current_hint;
public:
    /**
     * constructor
     */
    SudokuModel();

    /**
     * loads the model from a given file
     *
     * @param filename the name of the file
     */

    void load(std::string &filename);

    /**
     * loads the model from a given string
     *
     * @param s the string to load the model from
     */
    void load_from_string(const std::string &s);

    /**
     * clears the model
     */
    void clear();

    /**
     * generates a new sudoku
     */
    void generate();

    /**
     * returns the difficulty level of the
     * next puzzle generated
     *
     * @return the difficulty level of the
     * next puzzle generated
     */
    DifficultyLevel get_difficulty_level() const;

    /**
     * sets the difficulty level of the
     * next puzzle generated
     *
     * @param difficulty_level the difficulty level of the
     * next puzzle generated
     */
    void set_difficulty_level(DifficultyLevel difficulty_level);

    /**
     * returns true, if there is a command to be undone
     */
    bool can_undo() const;

    /**
     * returns true, if there is a command to be re-executed
     */
    bool can_redo() const;

    /**
     * undoes the previous command
     */
    void undo();

    /**
     * re-executes the previously undone command
     */
    void redo();

    /**
     * returns true, if the cell at a given index
     * contains a value
     *
     * @param idx the index of the cell to be tested
     * @return true, if the cell at a given index
     * contains a value
     */
    bool has_value(int idx) const;

    /**
     * returns the value of a cell at a given index
     * @param idx the index of the cell to get the value of
     * @return the value of the cell or 0, if the cell does
     *         not contain a value
     */
    int get_value(int idx) const;

    /**
     * returns true, if a given value is a valid choice
     * for a given cell
     *
     * @param idx the index of the cell to be tested
     * @param value the value to be tested
     *
     * @return true, if the value is a valid choice
     */
    bool has_choice(int idx, int value) const;

    /**
     * returns the index of the currently selected cell
     *
     * @return the index of the currently selected cell
     */
    int get_selected_cell() const;

    /**
     * sets the selected cell
     *
     * @param selected_cell the index of the cell to be selected (0..80)
     */
    void set_selected_cell(int selected_cell);

    /**
     * returns true, if the cell at a given row and
     * column is selected
     *
     * @param row the row of the cell to be tested (0..8)
     * @param column the row of the cell to be tested (0..8)
     * @return true, if the cell is selected
     */
    bool is_cell_selected(int row, int col) const;

    /**
     * returns the value of the highlighted choice
     *
     * @return the value of the highlighted choice
     */
    int get_highlighted_choice() const;

    /**
     * sets the highlighted choice
     *
     * @param highlighted_choice the choice to be highlighted
     */
    void set_highlighted_choice(int highlighted_choice);

    /**
     * returns the number of cells to be solved
     *
     * @return the number of cells to be solved
     */
    int get_todo() const;

    /**
     * returns the number of choices left
     *
     * @return the number of choices left
     */
    int get_num_choices() const;

    /**
     * returns true, if choices should be shown
     * @return true, if choices should be shown
     */
    bool get_show_choices() const;

    /**
     * sets whether choices should be displayed
     *
     * @param show_choices true, if choices should be displayed
     */
    void set_show_choices(bool show_choices);

    /**
     * finds the next hint if any and puts it into current_hint.
     */

    void find_next_hint();

    /**
     * applies the current hint
     */

    void apply_current_hint();

    /**
     * returns the current hint (the last hint found by find_next_hint)
     * @return the last hint found by find_next_hint
     */
    const Hint *get_current_hint() const;

    /**
     * prints the grid
     *
     * used to save the model to a file
     *
     * @param out the stream to print the model on
     */
    void print(std::ostream &out) const;

    /**
     * moves the current selection up
     */
    void move_selection_up();

    /**
     * moves the current selection down
     */
    void move_selection_down();

    /**
     * moves the current selection to the left
     */
    void move_selection_left();

    /**
     * moves the current selection to the right
     */
    void move_selection_right();

    /**
     * toggles the choice of the current cell
     *
     * @param value the choice to be toggled
     */
    void toggle_current_cell_choice(int value);

    /**
     * sets the value of the current cell
     *
     * @param value the value to be set
     */
    void set_current_cell_value(int value);

    /**
     * clears the value of the current cell
     */
    void clear_current_cell_value();

    /**
     * the signal emitted, if the model changes
     */
    type_signal_changed &signal_changed();

    /**
     * checks, if the model contains a solvable grid with one
     * unique solution.
     *
     * @return true, if the sudoku is valid
     */
    bool check() const;
private:
    void clear_current_hint();
private:
    SudokuModel(const SudokuModel &other) {
    }
    SudokuModel &operator =(const SudokuModel &other) {
        return *this;
    }
};

#endif
