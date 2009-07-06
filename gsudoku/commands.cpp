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

#include <algorithm>
#include "commands.hpp"
#include "../sudoku/grid.hpp"
#include "../sudoku/util.hpp"

Command::Command() {
}

Command::~Command() {
}

SetValueCommand::SetValueCommand(Grid &grid, int value, int idx) :
    grid(grid), backup(grid), value(value), idx(idx) {
}

void SetValueCommand::undo() {
    grid = backup;
}

void SetValueCommand::redo() {
    Cell &cell = grid[idx];
    if (cell.has_value())
        grid.clear_cell_value(cell);
    cell.set_value(value);
    grid.cleanup_choice(cell);
}

ClearValueCommand::ClearValueCommand(Grid &grid, int value, int idx) :
    grid(grid), backup(grid), value(value), idx(idx) {

}

void ClearValueCommand::undo() {
    grid = backup;
}

void ClearValueCommand::redo() {
    Cell &cell = grid[idx];
    if (cell.has_value())
        grid.clear_cell_value(cell);
}

RemoveChoiceCommand::RemoveChoiceCommand(Grid &grid, int value, int idx) :
    grid(grid), value(value), idx(idx) {
}

void RemoveChoiceCommand::undo() {
    grid[idx].add_choice(value);
}

void RemoveChoiceCommand::redo() {
    grid[idx].remove_choice(value);
}

AddChoiceCommand::AddChoiceCommand(Grid &grid, int value, int idx) :
    grid(grid), value(value), idx(idx) {
}

void AddChoiceCommand::undo() {
    grid[idx].remove_choice(value);
}

void AddChoiceCommand::redo() {
    grid[idx].add_choice(value);
}

UndoManager::UndoManager() {

}

UndoManager::~UndoManager() {
    std::for_each(undo_commands.begin(), undo_commands.end(),
            destroy<Command *> ());
    std::for_each(redo_commands.begin(), redo_commands.end(),
            destroy<Command *> ());
}

void UndoManager::undo() {
    if (undo_commands.empty())
        return;
    Command *command = undo_commands.back();
    undo_commands.pop_back();
    command->undo();
    redo_commands.push_back(command);
}

void UndoManager::redo() {
    if (redo_commands.empty())
        return;
    Command *command = redo_commands.back();
    redo_commands.pop_back();
    command->redo();
    undo_commands.push_back(command);
}

bool UndoManager::can_undo() const {
    return !undo_commands.empty();
}

bool UndoManager::can_redo() const {
    return !redo_commands.empty();
}

void UndoManager::add_undo_command(Command *command) {
    std::for_each(redo_commands.begin(), redo_commands.end(),
            destroy<Command *> ());
    redo_commands.clear();
    undo_commands.push_back(command);
}

void UndoManager::clear() {
    std::for_each(undo_commands.begin(), undo_commands.end(),
            destroy<Command *> ());
    std::for_each(redo_commands.begin(), redo_commands.end(),
            destroy<Command *> ());
    undo_commands.clear();
    redo_commands.clear();
}
