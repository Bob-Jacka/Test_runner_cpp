#ifndef STRAT_HPP
#define STRAT_HPP

#include <vector>
#include "../../Test_artifacts/declaration/Test_case.hpp"

namespace Strategy {
    /**
     * Abstract class for strategies in utility execution
     */
    class Strat {
    public:
        Strat() = default;

        virtual ~Strat() = default;

        [[nodiscard]] virtual std::vector<Check_runner::TA::Test_case> doAlgorithm(std::vector<Check_runner::TA::Test_case> &) const = 0;
    };
}

#endif
