#ifndef STRATEGYEXCEPTION_HPP
#define STRATEGYEXCEPTION_HPP

#include "Base_exception.hpp"
#include <string>

namespace Check_exceptions {
    class StrategyException final : public BaseException {
    public:
        explicit StrategyException(const std::string &);
    };

    inline StrategyException::StrategyException(const std::string &msg) {
        Utility::message_with_location(msg);
    }
}
#endif
