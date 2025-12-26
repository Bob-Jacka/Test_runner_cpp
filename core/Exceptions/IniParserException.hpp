#ifndef INIPARSEREXCEPTION_HPP
#define INIPARSEREXCEPTION_HPP
#include "Base_exception.hpp"

namespace Check_exceptions {
    class IniParserException final : BaseException {
    public:
        explicit IniParserException(int line, const std::string &msg, const char *filename);
    };

    inline IniParserException::IniParserException(const int line, const std::string &msg, const char *filename) {
        std::cout << "Error: " << msg << " at line " << line << " at file " << filename << std::endl;
    }
}
#endif
