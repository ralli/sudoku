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

#ifndef SUDOKUGENERATOR_HPP_
#define SUDOKUGENERATOR_HPP_

#include <vector>

class Grid;
class Choices;
class HintProducer;

class SudokuGenerator {
public:
    typedef enum {EASY, MEDIUM, HARD} Difficulty;
private:
    char field[81];
    std::vector<HintProducer *> hint_producers;
    Difficulty difficulty;
    int min_idx;
    int max_idx;
public:
    SudokuGenerator();
    ~SudokuGenerator();
    void generate(Grid &grid);
    Difficulty get_difficulty() const;
    void set_difficulty(Difficulty difficulty);
private:
    SudokuGenerator(const SudokuGenerator &other) {
    }
    SudokuGenerator &operator =(const SudokuGenerator &other) {
        return *this;
    }
    void do_generate(Grid &grid);
    bool generate_full(int idx);
    void fill_choices(int idx, Choices &choices) const;
    int random_choice(Choices &choices) const;
    void remove_fields(Grid &grid) const;
    bool check_difficulty(const Grid &grid);
};

#endif
