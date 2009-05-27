#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "util.hpp"
#include "range.hpp"
#include "grid.hpp"
#include "hint.hpp"
#include "hintconsumer.hpp"
#include "singlehint.hpp"
#include "hiddendouble.hpp"
#include "forcingchain.hpp"
#include "pointing.hpp"
#include "boxlinereduction.hpp"
#include "xwing.hpp"

int main(int argc, char *argv[]) {
#if 0
    std::string
    s =
    ".....39483.9..85....4.....25..9.......7.1.6.......7..16.....1....87..2.91753.....";
#endif
#if 0
    std::string
    s =
    "59.8.2.67.........6..7.3..1..9...1.....186.....7...3..8..4.5..2.........32.6.8.45";
#endif
#if 0
    std::string
    s =
    "..........9.7.6.2..3..1..5...2.6.3..76.5.1.49..9.4.8...2..7..3..5.9.4.8..........";
#endif
#if 0
    std::string
            s =
                    "385694..21465729832973814566148.3..97531.96.8829.6...147193826553...6.9796......4";

#endif
#if 1
    std::string s = "9..6.2..4.7.9.428...43789..4..5.9..2.9.....4.56.4.1.98..9145637735896421146...859";
#endif
    std::istringstream in(s);
    Grid grid;
    std::vector<HintProducer *> hintproducers;

    hintproducers.push_back(new SingleHintProducer());
    hintproducers.push_back(new HiddenDoubleHintProducer());
    hintproducers.push_back(new PointingHintProducer());
    hintproducers.push_back(new XWingHintProducer());
//    hintproducers.push_back(new BoxLineReductionHintProducer());
    hintproducers.push_back(new ForcingChainHintProducer());

    grid.load(in);
    grid.print(std::cout);
    grid.print_choices(std::cout);
    grid.print_status(std::cout);
    std::cout << std::endl;

    int iteration = 0;
    while (true) {
        ++iteration;
        SingleHintConsumer consumer;
        for (std::vector<HintProducer *>::const_iterator i =
                hintproducers.begin(); i != hintproducers.end(); ++i) {
            (*i)->find_hints(grid, consumer);
            if (consumer.has_hints())
                break;
        }
        if (!consumer.has_hints())
            break;
#if 0
        for (ListHintConsumer::iterator i = consumer.begin(); i
                != consumer.end(); ++i) {
            (*i)->print_description(std::cout);
            std::cout << std::endl;
            (*i)->apply();
        }
#endif
        std::cout << "iteration: " << iteration << std::endl;
        grid.print_choices(std::cout);
        grid.print_status(std::cout);
        std::cout << std::endl;
    }
    std::cout << "result: " << std::endl;
    grid.print_choices(std::cout);
    grid.print_status(std::cout);
    std::cout << std::endl;

    std::for_each(hintproducers.begin(), hintproducers.end(), destroy<HintProducer *>());
    return 0;
}

