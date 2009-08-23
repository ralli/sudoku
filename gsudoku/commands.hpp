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

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "../sudoku/grid.hpp"

/*!
 * \brief a command providing undo and redo operations.
 */
class Command {
public:
    Command();
    virtual ~Command();
    /*!
     * \brief undoes (reverts) the previous command
     */
    virtual void undo() = 0;
    /*!
     * \brief re-executes the previous undone command
     */
    virtual void redo() = 0;
private:
    Command(const Command &command) {
    }
    Command &operator =(const Command &command) {
        return *this;
    }
};

/*!
 * \brief a command undoing the setting of a value
 * in a given cell
 */
class SetValueCommand: public Command {
    Grid &grid;
    Grid backup;
    int value;
    int idx;
public:
    /*!
     * \brief constructor
     * \param grid the grid containing the cell
     * \param value the value, which has been set in the cell
     * \param idx the index (0..80) of the cell within the grid
     */
    SetValueCommand(Grid &grid, int value, int idx);
    void undo();
    void redo();
};

/*!
 * \brief a command undoing the clearing of a value
 * in a given cell
 */
class ClearValueCommand: public Command {
    Grid &grid;
    Grid backup;
    int value;
    int idx;
public:
    /*!
     * \brief constructor
     * \param grid the grid containing the cell
     * \param value the value, which has been removed from the cell
     * \param idx the index (0..80) of the cell within the grid
     */
    ClearValueCommand(Grid &grid, int value, int idx);
    void undo();
    void redo();
};

/*!
 * \brief a command undoing the removal of a choice
 * from a given cell
 */
class RemoveChoiceCommand: public Command {
    Grid &grid;
    int value;
    int idx;
public:
    /*!
     * \brief constructor
     * \param grid the grid containing the cell
     * \param value the choice, which has been removed from the cell
     * \param idx the index (0..80) of the cell within the grid
     */
    RemoveChoiceCommand(Grid &grid, int value, int idx);
    void undo();
    void redo();
};

/*!
 * \brief a command undoing the addition of a choice to
 * a given cell.
 */
class AddChoiceCommand: public Command {
    Grid &grid;
    int value;
    int idx;
public:
    /*!
     * \brief constructor
     * \param grid the grid containing the cell
     * \param value the choice, which has been removed from the cell
     * \param idx the index (0..80) of the cell within the grid
     */
    AddChoiceCommand(Grid &grid, int value, int idx);
    void undo();
    void redo();
};

class SolveSinglesCommand : public Command {
    Grid &grid;
    Grid backup;
public:
    /*!
     * \brief constructor
     * \param grid the grid
     */
    SolveSinglesCommand(Grid &grid);
    void undo();
    void redo();
};

/*!
 * \brief maintains a history of all undoable and redoable commands
 */
class UndoManager {
    std::vector<Command *> undo_commands;
    std::vector<Command *> redo_commands;
public:
    /*!
     * \brief constructor
     */
    UndoManager();

    /*!
     * \brief destructor
     */
    virtual ~UndoManager();

    /*!
     * \brief clears the command history
     */
    void clear();

    /*!
     * undoes the previous undoable command
     */
    void undo();

    /*!
     * \brief re- executes the previous undone command
     */
    void redo();

    /*!
     * \brief returns true, if there are any commands to be undone
     */
    bool can_undo() const;
    /*!
     * \brief returns true, if there are any commands to be re-executed
     */
    bool can_redo() const;

    /*!
     * \brief adds a new command to the history of undoable commands
     *
     * as a side effect, the list of re-executable commands will be cleared.
     */

    void add_undo_command(Command *command);
private:
    UndoManager(const UndoManager &other) {
    }
    UndoManager &operator =(const UndoManager &other) {
        return *this;
    }
};
#endif

