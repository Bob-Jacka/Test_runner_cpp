#include "../declaration/High_prior_strat.hpp"
#include <ranges>

auto Strategy::High_prior_strat::doAlgorithm(std::vector<TA::Test_case> input) const {
    auto out_vec = input | std::views::filter([]() -> auto {

    });
}

Strategy::High_prior_strat::~High_prior_strat() {
    //
}

Strategy::High_prior_strat::High_prior_strat() {
    //
}
