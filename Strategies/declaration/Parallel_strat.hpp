#ifndef PARALLEL_STRAT_HPP
#define PARALLEL_STRAT_HPP
#include <vector>

#include "Strat.hpp"

namespace Strategy {
    /**
     * Different version of random strategy.
     * Run in parallel test case execution.
     */
    class Parallel_strat final : public Strat {
    public:
        Parallel_strat();

        ~Parallel_strat() override;

        [[nodiscard]] auto doAlgorithm(std::vector<TA::Test_case>) const override;
    };
}

#endif
