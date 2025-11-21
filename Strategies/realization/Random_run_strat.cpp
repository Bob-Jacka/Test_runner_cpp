#include "../declaration/Random_run_strat.hpp"

#include <algorithm>
#include <random>

/**
 * Do test case shuffle and execute test cases
 * @param input
 * @return shuffled std::vector with test cases
 */
std::vector<TA::Test_case> Strategy::Random_run_strat::doAlgorithm(std::vector<TA::Test_case> &input) const {
    std::shuffle(input.begin(), input.end(), std::random_device{});
    return input;
}
