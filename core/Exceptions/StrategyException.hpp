#ifndef STRATEGYEXCEPTION_HPP
#define STRATEGYEXCEPTION_HPP

#include "Base_exception.hpp"
#include <string>

namespace Check_exceptions {
    class StrategyException final : public BaseException {
    public:
        explicit StrategyException(int, const std::string &, const char *);
    };

    inline StrategyException::StrategyException(const int line, const std::string &msg, const char *filename) {
        std::cout << "Error: " << msg << " at line " << line << " at file " << filename << std::endl;
    }
}
#endif
