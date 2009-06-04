#include <iostream>
#include "dancinglinks.hpp"

class MySolutionListener: public SolutionListener {
    std::ostream &out;
public:
    MySolutionListener(std::ostream &out);
    void solution_found(const std::vector<Node *> &rows);
};

MySolutionListener::MySolutionListener(std::ostream &out) :
    out(out) {
}

void MySolutionListener::solution_found(const std::vector<Node *> &rows) {
    out << "solution found: ";
    for (std::vector<Node *>::const_iterator i = rows.begin(); i
            != rows.end(); ++i) {
        out << "R" << (*i)->get_row() + 1;
        if (i + 1 != rows.end())
            out << ",";
    }
    out << std::endl;
}

void print(std::ostream &out, Solver &solver) {
    const Column *head = solver.get_head();
    const Node *c = head->get_right();
    while (c != head) {

        out << "C" << c->get_row() + 1 << ": size= "
                << static_cast<const Column *> (c)->get_size() << " ";
        const Node *r = c->get_down();
        while (r != c) {
            out << "R" << r->get_row() + 1;
            if (r->get_down() != c)
                out << ",";
            r = r->get_down();
        }
        c = c ->get_right();
        out << std::endl;
    }
}

int main(int argc, char **argv) {
    MySolutionListener listener(std::cout);
    Solver solver(&listener);
    int matrix[6][7] = { { 0, 0, 1, 0, 1, 1, 0 }, { 1, 0, 0, 1, 0, 0, 1 }, { 0,
            1, 1, 0, 0, 1, 0 }, { 1, 0, 0, 1, 0, 0, 0 },
            { 0, 1, 0, 0, 0, 0, 1 }, { 0, 0, 0, 1, 1, 0, 1 } };
    std::vector<Column *> columns;
    for (int i = 0; i < 7; ++i) {
        solver.add_column(i);
    }
    Column *head = solver.get_head();
    Node *c = head->get_right();
    while (c != head) {
        columns.push_back((static_cast<Column *> (c)));
        c = c->get_right();
    }
    for (int row = 0; row < 6; ++row) {
        Node *pfirst = 0;
        for (int col = 0; col < 7; ++col) {
            if (matrix[row][col]) {
                Node *node = columns[col]->create_child(row);
                if (pfirst) {
                    pfirst->add_sibling(node);
                } else {
                    pfirst = node;
                }
            }
        }
    }
    // solver.print_heads(std::cout);
    print(std::cout, solver);
    solver.solve();
    print(std::cout, solver);
    std::cout << "hallo" << std::endl;
    return 0;
}
