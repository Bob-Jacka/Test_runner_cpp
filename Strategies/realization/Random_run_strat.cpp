#include "../declaration/Random_run_strat.hpp"

#include <algorithm>
#include <random>

std::vector<TA::Test_case> Strategy::Random_run_strat::doAlgorithm(const std::vector<TA::Test_case> &input) const {
    std::shuffle(input.begin(), input.end(), std::random_device{});
    return input;
}
