#ifndef BASE_EXCEPTION_HPP
#define BASE_EXCEPTION_HPP

#include <exception>
#include <iostream>

import UtilFuncs_mod;

/**
 * Namespace for exceptions in check runner
 */
namespace Check_exceptions {
    class BaseException : public std::exception {
    public:
        [[nodiscard]] const char *what() const noexcept override;

        BaseException() = default;

        ~BaseException() override = default;
    };

    inline const char *BaseException::what() const noexcept {
        return exception::what();
    }
}

#endif
