#ifndef SUDOKUGENERATOR_HPP_
#define SUDOKUGENERATOR_HPP_

#include <vector>

class Grid;
class Choices;
class HintProducer;

class SudokuGenerator {
    char field[81];
    std::vector<HintProducer *> hint_producers;
public:
    SudokuGenerator();
    ~SudokuGenerator();
    void generate(Grid &grid);
private:
    SudokuGenerator(const SudokuGenerator &other) {
    }
    SudokuGenerator &operator =(const SudokuGenerator &other) {
        return *this;
    }
    void do_generate(Grid &grid);
    bool generate_full(int idx);
    void fill_choices(int idx, Choices &choices) const;
    int random_choice(Choices &choices) const;
    void remove_fields(Grid &grid) const;
    bool check_difficulty(const Grid &grid);
};

#endif
