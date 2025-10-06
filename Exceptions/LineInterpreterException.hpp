#ifndef LINEINTERPRETEREXCEPTION_HPP
#define LINEINTERPRETEREXCEPTION_HPP

#include "Base_exception.hpp"

class LineInterpreterException final : public BaseException {
    explicit LineInterpreterException(const std::string &);

    ~LineInterpreterException() override;
};

inline LineInterpreterException::LineInterpreterException(const std::string &msg) {
    utility::message_with_location(msg);
}

#endif
