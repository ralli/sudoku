#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <ctime>

#include "util.hpp"
#include "range.hpp"
#include "grid.hpp"
#include "hint.hpp"
#include "hintconsumer.hpp"
#include "singlehint.hpp"
#include "hiddendouble.hpp"
#include "simpleforcing.hpp"
#include "pointing.hpp"
#include "boxlinereduction.hpp"
#include "xwing.hpp"
#include "hiddentriple.hpp"
#include "forcingchain.hpp"
#include "nakeddouble.hpp"
#include "claiming.hpp"

bool solve(const std::string &s) {
    std::istringstream in(s);
    Grid grid;
    std::vector<HintProducer *> hintproducers;
    
    hintproducers.push_back(new SingleHintProducer());
    hintproducers.push_back(new NakedDoubleHintProducer());
    hintproducers.push_back(new HiddenDoubleHintProducer());
    hintproducers.push_back(new PointingHintProducer());
    hintproducers.push_back(new ClaimingHintProducer());
    hintproducers.push_back(new HiddenTripleHintProducer());
    hintproducers.push_back(new XWingHintProducer());
    hintproducers.push_back(new BoxLineReductionHintProducer());
    hintproducers.push_back(new SimpleForcingChainHintProducer());
    hintproducers.push_back(new ForcingChainHintProducer());

    grid.load(in);
    grid.print(std::cout);
    grid.print_choices(std::cout);
    grid.print_status(std::cout);
    std::cout << std::endl;

    int iteration = 0;
    while (true) {
        ++iteration;
        std::cout << "iteration: " << iteration << std::endl;
        SingleHintConsumer consumer;
        for (std::vector<HintProducer *>::const_iterator i =
            hintproducers.begin(); i != hintproducers.end(); ++i) {
                (*i)->find_hints(grid, consumer);
                if (consumer.has_hints())
                    break;
        }
        if (!consumer.has_hints())
            break;
        grid.print_choices(std::cout);
        grid.print_status(std::cout);
        std::cout << std::endl;
    }
    std::cout << "result: " << std::endl;
    grid.print_choices(std::cout);
    grid.print_status(std::cout);
    std::cout << std::endl;

    std::for_each(hintproducers.begin(), hintproducers.end(), destroy<
        HintProducer *> ());

    return grid.get_to_do() == 0;
}

int main(int argc, char *argv[]) {
    std::string filename = "top95.txt";

    if(argc > 1) {
        filename = argv[1];
    }

    std::ifstream in(filename.c_str());
    if(!in) {
        std::cerr << "cannot open file " << filename << std::endl;
        exit(1);
    }

    clock_t t1 = clock();

    std::string line;
    int i = 0;
    int success_count = 0;
    int failure_count = 0;
    while(getline(in, line)) {
        ++i;

        std::cout << i << ": " << line << std::endl;
        if(solve(line))
            ++success_count;
        else
            ++failure_count;
    }

    clock_t t2 = clock();
    double t = t2-t1;
    t /= CLOCKS_PER_SEC;
    std::cout << i << " sudokus success: " << success_count << " failures: " << failure_count << " time: " << t << " seconds" << std::endl;
    return 0;
}

