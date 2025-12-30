#ifndef STRATCONTEX_HPP
#define STRATCONTEX_HPP

#include <memory>

#include "Strat.hpp"

/**
 * Namespace for strategies
 */
namespace Strategy {
    /**
     * Header only class for strategy context
     */
    class StratContext {
        std::unique_ptr<Strat> strategy_;

    public:
        explicit StratContext() = default;

        void set_strategy(std::unique_ptr<Strat> &&strategy) {
            strategy_ = std::move(strategy);
        }

        std::unique_ptr<Strat> get_strat() {
            if (strategy_ == nullptr) {
                return nullptr;
            }
            return std::move(strategy_);
        }
    };
}

#endif
