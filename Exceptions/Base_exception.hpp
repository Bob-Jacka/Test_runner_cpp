#ifndef BASE_EXCEPTION_HPP
#define BASE_EXCEPTION_HPP

import UtilFuncs_mod;

#include <exception>

class BaseException : public std::exception {
public:
    [[nodiscard]] const char * what() const noexcept override;

    BaseException();

    ~BaseException() override;
};

inline const char * BaseException::what() const noexcept {
    return exception::what();
}

#endif
