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

#ifndef DANCINGLINKS_HPP_
#define DANCINGLINKS_HPP_

#include <vector>

#pragma warning(disable:4355)

class NodeFactory;
class Column;
class Node {
    int row;
    Node *left;
    Node *right;
    Node *up;
    Node *down;
    Column *column;
public:
    Node(Column *column, int row = -1);
    int get_row() const;
    const Node *get_left() const;
    const Node *get_right() const;
    const Node *get_up() const;
    const Node *get_down() const;
    const Column *get_column() const;
    Node *get_left();
    Node *get_right();
    Node *get_up();
    Node *get_down();
    Column *get_column();
    void set_left(Node *node);
    void set_right(Node *node);
    void set_up(Node *node);
    void set_down(Node *node);
    void set_column(Column *column);
    void add_sibling(Node *node);
private:
    Node(const Node &other) {
    }
    Node &operator =(const Node &other) {
        return *this;
    }
};

class Column: public Node {
    int size;
public:
    Column(int idx = -1);
    int get_size() const;
    void set_size(int size);
    void add_child(Node *node);
    Node *create_child(int row);
};

class NodeFactory {
private:
    std::vector<Node *> nodes;
public:
    NodeFactory();
    virtual ~NodeFactory();
    Node *create_node(Column *column, int row = -1);
    Node *create_child(Column *column, int row = -1);
    Column *create_column(int row = -1);
private:
    NodeFactory(const NodeFactory &other) {
    }
    NodeFactory &operator =(const NodeFactory &other) {
        return *this;
    }

};

class SolutionListener {
public:
    virtual ~SolutionListener();
    virtual bool solution_found(const std::vector<Node *> &rows) = 0;
};

class Solver {
    NodeFactory *node_factory;
    Column *head;
    SolutionListener *solution_listener;
    std::vector<Node *> result;
public:
    Solver(NodeFactory *node_factory, SolutionListener *solution_listener);
    Column *add_column(int idx);
    Column *get_head();
    const Column *get_head() const;
    bool solve();
private:
    Solver(const Solver &other) {
    }
    Solver &operator =(const Solver &other) {
        return *this;
    }
    void cover(Column *column);
    void uncover(Column *column);
    Column *choose_column();
};

inline Node::Node(Column *column, int row) :
    row(row), left(this), right(this), up(this), down(this), column(column) {

}

inline int Node::get_row() const {
    return row;
}

inline const Node *Node::get_left() const {
    return left;
}

inline const Node *Node::get_right() const {
    return right;
}

inline const Node *Node::get_up() const {
    return up;
}

inline const Node *Node::get_down() const {
    return down;
}

inline const Column *Node::get_column() const {
    return column;
}

inline Node *Node::get_left() {
    return left;
}

inline Node *Node::get_right() {
    return right;
}

inline Node *Node::get_up() {
    return up;
}

inline Node *Node::get_down() {
    return down;
}

inline Column::Column(int idx) :
    Node(this, idx), size(0) {
}

inline Column *Node::get_column() {
    return column;
}

inline void Node::set_left(Node *left) {
    this->left = left;
}

inline void Node::set_right(Node *right) {
    this->right = right;
}

inline void Node::set_up(Node *up) {
    this->up = up;
}

inline void Node::set_down(Node *down) {
    this->down = down;
}

inline void Node::set_column(Column *column) {
    this->column = column;
}

inline void Node::add_sibling(Node *node) {
    left->set_right(node);
    node->set_left(left);
    this->set_left(node);
    node->set_right(this);
}

inline int Column::get_size() const {
    return size;
}

inline void Column::set_size(int size) {
    this->size = size;
}

inline Column *Solver::get_head() {
    return head;
}

inline const Column *Solver::get_head() const {
    return head;
}

inline NodeFactory::NodeFactory() {
}

inline Node *NodeFactory::create_node(Column *column, int row) {
    Node *node = new Node(column, row);
    nodes.push_back(node);
    return node;
}
inline Node *NodeFactory::create_child(Column *column, int row) {
    Node *node = create_node(column, row);
    column->add_child(node);
    return node;
}

inline Column *NodeFactory::create_column(int row) {
    Column *column = new Column(row);
    nodes.push_back(column);
    return column;
}

#endif
