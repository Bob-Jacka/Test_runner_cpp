#ifndef RANDOM_RUN_STRAT_HPP
#define RANDOM_RUN_STRAT_HPP
#include "Strat.hpp"

namespace Strategy {
    class Random_run_strat final : public Strat {
    public:
        Random_run_strat() = default;

        ~Random_run_strat() override = default;

        [[nodiscard]] std::vector<Check_runner::TA::Test_case> doAlgorithm(std::vector<Check_runner::TA::Test_case>&) const override;
    };
}
#endif
