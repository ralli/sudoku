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

#ifndef HINT_HPP_
#define HINT_HPP_

#include <iosfwd>

class HintConsumer;
class Grid;

/**
 * A hint is a single step to the solution of a sudoku.
 *
 * Hints are typically created by a Hint producer. A hint producer is
 * a strategy to find a hint. One example of such a strategy is the
 * naked single strategy which tries to find cells with only one single
 * choice left.
 *
 * Hints are normally processed by a hint consumer which is responsible
 * to apply the hint to the sudoku grid. Applying means, that the new
 * field values are set and all impossible remaining choices are removed.
 */

class Hint {
public:
    /**
     * Constructor
     */
    Hint() {
    }

    /**
     * Destructor
     */
    virtual ~Hint() {
    }

    /**
     * Applies the hint to the sudoku.
     */
    virtual void apply(Grid &grid) = 0;

    /**
     * Prints a description of the hint wich can be interpreted by a human user.
     *
     * @param out the stream to print oon
     */
    virtual void print_description(std::ostream &out) const = 0;

    /**
     * Gets the hints name.
     *
     * The hints name is the name of the strategy used to find the hint
     * (ex. Naked Single)
     */
    virtual const char *get_name() const = 0;
private:
    /**
     * Private copy constructor.
     *
     * @param other the hint to be prevented from being copied
     */
    Hint(const Hint &other) {
    }

    /**
     * Private assignment operator.
     *
     * @param other the hint to be prevented from being copied
     */
    Hint & operator =(const Hint &other) {
        return *this;
    }
};

/*!
 * \brief one of the various strategies to create hints.
 */
class HintProducer {
public:
    /*!
     * \brief searches the grid for hints and feeds them into the hintconsumer.
     * \param grid the grid to be searched for hints
     * \param consumer the hint consumer to feed the found hints into
     */
    virtual void find_hints(Grid &grid, HintConsumer &consumer) = 0;
};

/*!
 * \brief one of the various strategies to process the
 * hints found during the solution of a sudoku.
 */
class HintConsumer {
public:
    /*!
     * \brief consumes a hint.
     * \param hint the hint to be consumed.
     *             it is the responsibility of the consume_hint method to delete (free)
     *             the hint when it is not neede anymore.
     * \returns true, if if the hintproducer should continue searching for hints.
     */
    virtual bool consume_hint(Hint *hint) = 0;
    /*!
     * \brief returns true, if the hintproducer should continue searching for hints.
     * \return true, if the hintproducer should continue searching for hints.
     */
    virtual bool wants_more_hints() const = 0;
};

#endif
