#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "dancinglinks.hpp"

Solver::Solver(SolutionListener *solution_listener) :
    head(new Column(-1)), solution_listener(solution_listener) {
}

void Solver::add_column(int idx) {
    Column *column = new Column(idx);
    Node *left = head->get_left();
    left->set_right(column);
    column->set_left(left);
    column->set_right(head);
    head->set_left(column);
}

void Solver::solve() {
    if (head->get_right() == head) {
        solution_listener->solution_found(result);
        return;
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
        solve();
        result.pop_back();
        row_column = row->get_left();
        while (row_column != row) {
            uncover(row_column->get_column());
            row_column = row_column->get_left();
        }
        row = row->get_down();
    }
    uncover(column);
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

Node *Column::create_child(int row) {
    Node *node = new Node(this, row);
    add_child(node);
    return node;
}

SolutionListener::~SolutionListener() {

}
