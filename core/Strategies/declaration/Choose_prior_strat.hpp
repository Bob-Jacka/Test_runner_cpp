#ifndef CHOOSE_PRIOR_STRAT_HPP
#define CHOOSE_PRIOR_STRAT_HPP
#include "Strat.hpp"

namespace Strategy {
    /**
     * Example of usage
     * --strat=choose<low> - will give only low priority test cases
     * --strat=choose<> or --strat=choose - will give default count of test cases
     */
    class Choose_prior_strat final : public Strat {
    public:
        ~Choose_prior_strat() override = default;

        [[nodiscard]] std::vector<Check_runner::TA::Test_case> doAlgorithm(std::vector<Check_runner::TA::Test_case> &) const override;
    };
}

#endif
