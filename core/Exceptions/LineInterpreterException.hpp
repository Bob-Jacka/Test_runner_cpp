#ifndef LINEINTERPRETEREXCEPTION_HPP
#define LINEINTERPRETEREXCEPTION_HPP

#include "Base_exception.hpp"

namespace Check_exceptions {
    class LineInterpreterException final : public BaseException {
    public:
        explicit LineInterpreterException(int line, const std::string &msg, const char *filename);
    };

    inline LineInterpreterException::LineInterpreterException(const int line, const std::string &msg, const char *filename) {
        std::cout << "Error: " << msg << " at line " << line << " at file " << filename << std::endl;
    }
}

#endif
