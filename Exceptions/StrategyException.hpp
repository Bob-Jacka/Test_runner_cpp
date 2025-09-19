#ifndef STRATEGYEXCEPTION_HPP
#define STRATEGYEXCEPTION_HPP
#include "Base_exception.hpp"

class StrategyException final : public BaseException {
public:
    explicit StrategyException(const std::string &);
};

inline StrategyException::StrategyException(const std::string &msg) {
    utility::message_with_location(msg);
}
#endif
