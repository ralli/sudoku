#ifndef DLXSOLVER_HPP
#define DLXSOLVER_HPP

#include "dancinglinks.hpp"

class Grid;
class Cell;

class DlxSudokuSolver: public SolutionListener {
public:
    DlxSudokuSolver();
    virtual ~DlxSudokuSolver();
    void solve(Grid &grid);
    void solution_found(const std::vector<Node *> &rows);
private:
    void init_solver(NodeFactory &node_factory, Solver &solver, Grid &grid);
    void add_nodes(const Cell &cell, int value, std::vector<Column *> &columns,
            NodeFactory &node_factory);
    DlxSudokuSolver(const DlxSudokuSolver &other) {
    }
    DlxSudokuSolver &operator =(const DlxSudokuSolver &other) {
        return *this;
    }
};
#endif
