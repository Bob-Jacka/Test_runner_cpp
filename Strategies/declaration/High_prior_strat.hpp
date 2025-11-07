#ifndef HIGH_PRIOR_STRAT_HPP
#define HIGH_PRIOR_STRAT_HPP
#include "Strat.hpp"

namespace Strategy {
    class High_prior_strat final : public Strat {
    public:
        High_prior_strat() = default;

        ~High_prior_strat() override = default;

        [[nodiscard]] std::vector<TA::Test_case> doAlgorithm(const std::vector<TA::Test_case>&) const override;
    };
}
#endif
