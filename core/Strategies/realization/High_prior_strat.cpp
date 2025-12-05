#include "../declaration/High_prior_strat.hpp"

/**
 * Do high priority algorithm on test cases suit (run only high priority tests - high or critical).
 * @param input input vector of test cases.
 * @return modified vector of test cases.
 */
std::vector<Check_runner::TA::Test_case> Strategy::High_prior_strat::doAlgorithm(std::vector<Check_runner::TA::Test_case> &input) const {
    auto filter_func = [&](const Check_runner::TA::Test_case &elem) -> bool {
        const auto priority = elem.get_priority();
        const auto severity = elem.get_severity();
        return (priority == Check_runner::TA::Priority::High or priority == Check_runner::TA::Priority::Critical) and (static_cast<int>(severity) >= 1);
    };
    auto view = std::ranges::filter_view{input, filter_func};
    std::vector<Check_runner::TA::Test_case> to_return{};
    for (const auto& elem: view) {
        to_return.push_back(elem);
    }
    return to_return;
}
