#ifndef RANDOM_RUN_STRAT_HPP
#define RANDOM_RUN_STRAT_HPP
#include "Strat.hpp"

namespace Strategy {
    class Random_run_strat final : public Strat {
    public:
        Random_run_strat();

        ~Random_run_strat() override;

        [[nodiscard]] auto doAlgorithm(std::vector<TA::Test_case>) const override;
    };
}
#endif
