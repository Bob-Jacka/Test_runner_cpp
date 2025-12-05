#ifndef USUALSTRAT_HPP
#define USUALSTRAT_HPP
#include "Strat.hpp"

namespace Strategy {
    class Usual_strat final : public Strat {
    public:
        [[nodiscard]] std::vector<Check_runner::TA::Test_case> doAlgorithm(std::vector<Check_runner::TA::Test_case> &) const override;
    };
}
#endif
