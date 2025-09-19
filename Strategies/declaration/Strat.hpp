#ifndef STRAT_HPP
#define STRAT_HPP

#include "Data/Constants.hpp"
#include <vector>
#include "../../Test_artifacts/declaration/Test_case.hpp"

namespace Strategy {
    /**
     * Abstract class for strategies
     */
    class Strat {
    public:
        Strat() = default;

        virtual ~Strat() = default;

        [[nodiscard]] virtual auto doAlgorithm(std::vector<TA::Test_case>) const = 0;
    };
}

#endif
