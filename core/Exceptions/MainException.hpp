#ifndef MAINEXCEPTION_HPP
#define MAINEXCEPTION_HPP

#include "Base_exception.hpp"

namespace Check_exceptions {
    class MainException final : public BaseException {
    public:
        explicit MainException(int line, const std::string &msg, const char *filename);
    };

    inline MainException::MainException(const int line, const std::string &msg, const char *filename) {
        std::cout << "Error: " << msg << " at line " << line << " at file " << filename << std::endl;
    }
}

#endif
