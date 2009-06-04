#ifndef DANCINGLINKS_HPP_
#define DANCINGLINKS_HPP_

#include <vector>

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

class Solver {
    Column *head;
    std::vector<Node *> result;
public:
    Solver();
    void add_column(int idx);
    Column *get_head();
    const Column *get_head() const;
    void solve();
private:
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
    Node(this, idx) {
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
#endif
