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
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include "dancinglinks.hpp"
#include "util.hpp"

Solver::Solver(NodeFactory *node_factory, SolutionListener *solution_listener) :
    node_factory(node_factory), head(node_factory->create_column(-1)),
            solution_listener(solution_listener) {
}

Column * Solver::add_column(int idx) {
    Column *column = node_factory->create_column(idx);
    Node *left = head->get_left();
    left->set_right(column);
    column->set_left(left);
    column->set_right(head);
    head->set_left(column);
    return column;
}

bool Solver::solve() {
    if (head->get_right() == head) {
        return solution_listener->solution_found(result);
    }
    Column *column = choose_column();
    cover(column);
    Node *row = column->get_down();
    while (row != column) {
        result.push_back(row);
        Node *row_column = row->get_right();
        while (row_column != row) {
            cover(row_column->get_column());
            row_column = row_column->get_right();
        }
        if(solve())
            return true;
        result.pop_back();
        row_column = row->get_left();
        while (row_column != row) {
            uncover(row_column->get_column());
            row_column = row_column->get_left();
        }
        row = row->get_down();
    }
    uncover(column);
    return false;
}

void Solver::cover(Column *column) {
    column->get_right()->set_left(column->get_left());
    column->get_left()->set_right(column->get_right());
    Node *i = column->get_down();
    while (i != column) {
        Node *j = i->get_right();
        while (j != i) {
            j->get_down()->set_up(j->get_up());
            j->get_up()->set_down(j->get_down());
            j->get_column()->set_size(j->get_column()->get_size() - 1);
            j = j->get_right();
        }
        i = i->get_down();
    }
}

void Solver::uncover(Column *column) {
    Node *i = column->get_up();
    while (i != column) {
        Node *j = i->get_left();
        while (j != i) {
            j->get_column()->set_size(j->get_column()->get_size() + 1);
            j->get_down()->set_up(j);
            j->get_up()->set_down(j);
            j = j->get_left();
        }
        i = i->get_up();
    }
    column->get_left()->set_right(column);
    column->get_right()->set_left(column);
}

Column *Solver::choose_column() {
    int min = std::numeric_limits<int>::max();
    Node *found = 0;
    Node *c = head->get_right();
    while (c != head) {
        if (c->get_column()->get_size() < min) {
            min = c->get_column()->get_size();
            found = c;
            if(min == 1) {
                /*
                 * the solution cannot get better.
                 * on my machine it is faster to have the
                 * extra comparison here.
                 */
                return found->get_column();
            }
        }
        c = c->get_right();
    }
    return found ? found->get_column() : 0;
}

void Column::add_child(Node *node) {
    Node *up = get_up();
    up->set_down(node);
    node->set_up(up);
    node->set_down(this);
    this->set_up(node);
    ++size;
}

SolutionListener::~SolutionListener() {

}

NodeFactory::~NodeFactory() {
    std::for_each(nodes.begin(), nodes.end(), destroy<Node *> ());
}
