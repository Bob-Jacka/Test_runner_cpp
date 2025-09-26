#include "../declaration/High_prior_strat.hpp"

/**
 * Do high priority algorithm on test cases suit (run only high priority tests - high or critical).
 * @param input input vector of test cases.
 * @return modified vector of test cases.
 */
auto Strategy::High_prior_strat::doAlgorithm(std::vector<TA::Test_case> input) const noexcept -> const auto {
    auto filter_func = [&](auto elem) -> bool {
        auto priority = elem.get_priority();
        return priority == TA::Priority::High or priority == TA::Priority::Critical;
    };
    auto view = std::ranges::filter_view<std::vector<TA::Test_case>, auto>{input, filter_func};
    return view;
}

Strategy::High_prior_strat::~High_prior_strat() {
    //
}

Strategy::High_prior_strat::High_prior_strat() {
    //
}
