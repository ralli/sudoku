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
#include "simpleforcing.hpp"
#include "pointing.hpp"
#include "boxlinereduction.hpp"
#include "xwing.hpp"
#include "hiddentriple.hpp"
#include "forcingchain.hpp"

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
#if 1
    std::string
    s =
    "..........9.7.6.2..3..1..5...2.6.3..76.5.1.49..9.4.8...2..7..3..5.9.4.8..........";
#endif
#if 0
    std::string
    s =
    "385694..21465729832973814566148.3..97531.96.8829.6...147193826553...6.9796......4";

#endif
#if 0
    std::string
            s =
                    "9..6.2..4.7.9.428...43789..4..5.9..2.9.....4.56.4.1.98..9145637735896421146...859";
#endif
#if 0
    std::string s = "37.4.81.....9.37.494.1...8342......5...5.4...8......46.1..49...5.96..4....42..931";
#endif
#if 0
    std::string s = "4..27.6..798156234.2.84...7237468951849531726561792843.82.15479.7..243....4.87..2";

#endif
#if 0
   std::string s = ".14..9...76.2.....5..7..........1.4..57.6.91..2.5..........8..6.....5.38...9..15.";
#endif
#if 0
   std::string s = "....4.2...46..1.3..386.2........8.1.3.......2.9.4........5.946..8.1..75...5.6....";
#endif
#if 0
   /* easter monster */
   std::string s = "1.......2.9.4...5...6...7...5.9.3.......7.......85..4.7.....6...3...9.8...2.....1";
#endif
    std::istringstream in(s);
    Grid grid;
    std::vector<HintProducer *> hintproducers;

    hintproducers.push_back(new SingleHintProducer());
    hintproducers.push_back(new HiddenDoubleHintProducer());
    hintproducers.push_back(new PointingHintProducer());
    hintproducers.push_back(new HiddenTripleHintProducer());
    hintproducers.push_back(new XWingHintProducer());
    hintproducers.push_back(new BoxLineReductionHintProducer());
//    hintproducers.push_back(new SimpleForcingChainHintProducer());
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
#if 0
        for (ListHintConsumer::iterator i = consumer.begin(); i
                != consumer.end(); ++i) {
            (*i)->print_description(std::cout);
            std::cout << std::endl;
            (*i)->apply();
        }
#endif
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

    return 0;
}

