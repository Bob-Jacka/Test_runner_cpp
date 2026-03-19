#ifndef LINEINTERPRETEREXCEPTION_HPP
#define LINEINTERPRETEREXCEPTION_HPP

#include "Base_exception.hpp"

namespace Check_exceptions {
    class LineParserException final : public BaseException {
        public:
            explicit LineParserException(int line, const char *msg, const char *filename);
    };

    inline LineParserException::LineParserException(const int   line, const char *msg,
                                                              const char *filename) {
        std::cout << "Error: " << msg << " at line " << line << " at file " << filename << std::endl;
    }
}

#endif
