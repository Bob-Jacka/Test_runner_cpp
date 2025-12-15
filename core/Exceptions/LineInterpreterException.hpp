#ifndef LINEINTERPRETEREXCEPTION_HPP
#define LINEINTERPRETEREXCEPTION_HPP

#include "Base_exception.hpp"

namespace Check_exceptions {
    class LineInterpreterException final : public BaseException {
    public:
        explicit LineInterpreterException(const std::string &);
    };

    inline LineInterpreterException::LineInterpreterException(const std::string &msg) {
        Utility::message_with_location(msg);
    }
}

#endif
