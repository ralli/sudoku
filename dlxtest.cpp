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
    for (std::vector<Node *>::const_iterator i = rows.begin(); i != rows.end(); ++i) {
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
    NodeFactory node_factory;
    Solver solver(&node_factory, &listener);
    int matrix[6][7] = { { 0, 0, 1, 0, 1, 1, 0 }, { 1, 0, 0, 1, 0, 0, 1 }, { 0,
            1, 1, 0, 0, 1, 0 }, { 1, 0, 0, 1, 0, 0, 0 },
            { 0, 1, 0, 0, 0, 0, 1 }, { 0, 0, 0, 1, 1, 0, 1 } };
    std::vector<Column *> columns;
    for (int i = 0; i < 7; ++i) {
        columns.push_back(solver.add_column(i));
    }
    for (int row = 0; row < 6; ++row) {
        Node *pfirst = 0;
        for (int col = 0; col < 7; ++col) {
            if (matrix[row][col]) {
                Node *node = node_factory.create_child(columns[col], row);
                if (pfirst) {
                    pfirst->add_sibling(node);
                } else {
                    pfirst = node;
                }
            }
        }
    }
    print(std::cout, solver);
    solver.solve();
    print(std::cout, solver);
    return 0;
}
