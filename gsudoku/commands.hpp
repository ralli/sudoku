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

class Command {
public:
    Command();
    virtual ~Command();
    virtual void undo() = 0;
    virtual void redo() = 0;
private:
    Command(const Command &command) {}
    Command &operator = (const Command &command) { return *this; }
};

class SetValueCommand: public Command {
    Grid &grid;
    Grid  backup;
    int   value;
    int   idx;
public:
    SetValueCommand(Grid &grid, int value, int idx);
    void undo();
    void redo();
};

class ClearValueCommand: public Command {
    Grid &grid;
    Grid  backup;
    int value;
    int idx;
public:
    ClearValueCommand(Grid &grid, int value, int idx);
    void undo();
    void redo();
};

class RemoveChoiceCommand: public Command {
    Grid &grid;
    int value;
    int idx;
public:
    RemoveChoiceCommand(Grid &grid, int value, int idx);
    void undo();
    void redo();
};

class AddChoiceCommand: public Command {
    Grid &grid;
    int value;
    int idx;
public:
    AddChoiceCommand(Grid &grid, int value, int idx);
    void undo();
    void redo();
};

class UndoManager {
    std::vector<Command *> undo_commands;
    std::vector<Command *> redo_commands;
public:
    UndoManager();
    virtual ~UndoManager();
    void clear();
    void undo();
    void redo();
    bool can_undo() const;
    bool can_redo() const;
    void add_undo_command(Command *command);
private:
    UndoManager(const UndoManager &other) {}
    UndoManager &operator = (const UndoManager &other) { return *this;}
};
#endif

