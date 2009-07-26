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

#include "sudokusolver.hpp"
#include "util.hpp"
#include "range.hpp"
#include "grid.hpp"
#include "hint.hpp"
#include "hintconsumer.hpp"
#include "singlehint.hpp"
#include "hiddendouble.hpp"
#include "simpleforcing.hpp"
#include "pointing.hpp"
#include "boxlinereduction.hpp"
#include "xwing.hpp"
#include "hiddentriple.hpp"
#include "forcingchain.hpp"
#include "nakeddouble.hpp"
#include "claiming.hpp"
#include "swordfish.hpp"
#include "xywing.hpp"
#include "nakedsingle.hpp"

SudokuSolver::SudokuSolver() {
    hintproducers.push_back(new NakedSingleHintProducer());
    hintproducers.push_back(new SingleHintProducer());
    hintproducers.push_back(new NakedDoubleHintProducer());
    hintproducers.push_back(new HiddenDoubleHintProducer());
    hintproducers.push_back(new PointingHintProducer());
    hintproducers.push_back(new ClaimingHintProducer());
    // hintproducers.push_back(new BoxLineReductionHintProducer());
    hintproducers.push_back(new HiddenTripleHintProducer());
    hintproducers.push_back(new XWingHintProducer());
    hintproducers.push_back(new XYWingHintProducer());
    hintproducers.push_back(new SwordfishHintProducer());
    hintproducers.push_back(new ForcingChainHintProducer());
}

SudokuSolver::~SudokuSolver() {
    std::for_each(hintproducers.begin(), hintproducers.end(), destroy<
            HintProducer *> ());
}

void SudokuSolver::solve(Grid &grid, HintConsumer &consumer) {
    while (true) {
        for (std::vector<HintProducer *>::const_iterator i =
                hintproducers.begin(); i != hintproducers.end(); ++i) {
            (*i)->find_hints(grid, consumer);
            if (!consumer.wants_more_hints())
                break;
        }
        if (!consumer.wants_more_hints())
            break;
    }
}

class FindNextHintConsumer: public HintConsumer {
private:
    Hint *hint;
public:
    FindNextHintConsumer();
    bool consume_hint(Hint *hint);
    bool wants_more_hints() const;
    Hint *get_hint();
};

FindNextHintConsumer::FindNextHintConsumer() :
    hint(0) {
}

bool FindNextHintConsumer::consume_hint(Hint *hint) {
    this->hint = hint;
    return wants_more_hints();
}

bool FindNextHintConsumer::wants_more_hints() const {
    return hint == 0;
}

Hint *FindNextHintConsumer::get_hint() {
    return hint;
}

Hint *SudokuSolver::find_next_hint(Grid &grid) {
    FindNextHintConsumer consumer;

    for (std::vector<HintProducer *>::const_iterator i = hintproducers.begin(); i
            != hintproducers.end(); ++i) {
        (*i)->find_hints(grid, consumer);
        if (!consumer.wants_more_hints())
            break;
    }

    return consumer.get_hint();
}
