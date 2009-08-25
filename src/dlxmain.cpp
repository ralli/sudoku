#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "grid.hpp"
#include "dlxsolver.hpp"

class MySolutionListener: public SolutionListener {
public:
    bool solution_found(const std::vector<Node *> &rows);
};

bool MySolutionListener::solution_found(const std::vector<Node *> &rows) {
    std::cout << std::endl;
    Grid grid;
    for (std::vector<Node *>::const_iterator i = rows.begin(); i != rows.end(); ++i) {
        int idx = (*i)->get_row();
        int value = idx % 9 + 1;
        idx /= 9;
        grid[idx].set_value(value);
        grid[idx].clear_choices();
    }
    grid.print(std::cout);
    std::cout << std::endl;
    return false;
}

void solve(const std::string &s) {
    std::auto_ptr<MySolutionListener> p(new MySolutionListener());

    std::istringstream in(s);
    Grid grid;
    grid.load(in);
    grid.print(std::cout);
    std::cout << std::endl;
    DlxSudokuSolver solver(p.get());
    solver.solve(grid);
}

int main(int argc, char *argv[]) {
    std::string filename("top1465.txt");

    if (argc > 1) {
        filename = argv[1];
    }

    std::ifstream in(filename.c_str());
    if (!in) {
        std::cerr << "cannot open file " << filename << std::endl;
        return 1;
    }
    std::string line;
    while (getline(in, line)) {
        solve(line);
    }
    return 0;
}
