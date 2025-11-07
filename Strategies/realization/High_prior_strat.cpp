#include "../declaration/High_prior_strat.hpp"

/**
 * Do high priority algorithm on test cases suit (run only high priority tests - high or critical).
 * @param input input vector of test cases.
 * @return modified vector of test cases.
 */
std::vector<TA::Test_case> Strategy::High_prior_strat::doAlgorithm(const std::vector<TA::Test_case> &input) const {
    auto filter_func = [&](const TA::Test_case &elem) -> bool {
        const auto priority = elem.get_priority();
        return priority == TA::Priority::High or priority == TA::Priority::Critical;
    };
    auto view = std::ranges::filter_view{input, filter_func};
    return std::vector<TA::Test_case>{view.begin(), view.end()};
}
