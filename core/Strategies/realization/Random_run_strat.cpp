#include "../declaration/Random_run_strat.hpp"

#include <algorithm>
#include <random>

/**
 * Do test case shuffle and execute test cases
 * @param input_vector input vector data
 * @return shuffled std::vector with test cases
 */
std::vector<Check_runner::TA::Test_case>
Strategy::Random_run_strat::doAlgorithm(std::vector<Check_runner::TA::Test_case> &input_vector) const {
    std::ranges::shuffle(input_vector, std::random_device{});
    return input_vector;
}
