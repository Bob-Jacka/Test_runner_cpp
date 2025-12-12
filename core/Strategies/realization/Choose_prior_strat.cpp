#include "../declaration/Choose_prior_strat.hpp"
#include <ranges>

std::vector<Check_runner::TA::Test_case> Strategy::Choose_prior_strat::doAlgorithm(std::vector<Check_runner::TA::Test_case> &input_ts) const {
    auto filter_func = [&](const Check_runner::TA::Test_case &elem) -> bool {
        const auto priority = elem.get_priority();
        const auto severity = elem.get_severity();
        return (priority == Check_runner::TA::Priority::High or priority == Check_runner::TA::Priority::Critical) and (static_cast<int>(severity) >= 1);
    };
    auto view = std::ranges::filter_view{input_ts, filter_func};
    std::vector<Check_runner::TA::Test_case> to_return{};
    for (auto& elem: view) {
        to_return.push_back(elem);
    }
    return to_return;
}
