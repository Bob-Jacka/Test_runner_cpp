#ifndef STRATCONTEX_HPP
#define STRATCONTEX_HPP
#include <iostream>
#include <memory>
#include <bits/ostream.tcc>

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
        explicit StratContext(std::unique_ptr<Strat> &&strategy = {}) : strategy_(std::move(strategy)) {
        }

        void set_strategy(std::unique_ptr<Strat> &&strategy) {
            strategy_ = std::move(strategy);
        }

        std::unique_ptr<Strat> get_strat() {
            return std::move(strategy_);
        }
    };
}

#endif
