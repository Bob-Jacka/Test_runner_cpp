#include "../declaration/Random_run_strat.hpp"

#include <algorithm>
#include <random>

auto Strategy::Random_run_strat::doAlgorithm(std::vector<TA::Test_case> input) const -> auto {
    std::shuffle(input.begin(), input.end(), std::random_device{});
    return input;
}

Strategy::Random_run_strat::~Random_run_strat() {
    //
}

Strategy::Random_run_strat::Random_run_strat() {
    //
}
