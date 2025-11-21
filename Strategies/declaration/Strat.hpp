#ifndef STRAT_HPP
#define STRAT_HPP

#include <vector>
#include <ranges>
#include "../../Test_artifacts/declaration/Test_case.hpp"

namespace Strategy {
    /**
     * Abstract class for strategies in utility execution
     */
    class Strat {
    public:
        Strat() = default;

        virtual ~Strat() = default;

        [[nodiscard]] virtual std::vector<TA::Test_case> doAlgorithm(std::vector<TA::Test_case> &) const = 0;
    };
}

#endif
