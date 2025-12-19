#ifndef TESTARTIFACTEXCEPTION_HPP
#define TESTARTIFACTEXCEPTION_HPP

#include "Base_exception.hpp"
#include <string>

namespace Check_exceptions {
    class TestArtifactException final : public BaseException {
    public:
        explicit TestArtifactException(int, const std::string &, const char *);
    };

    inline TestArtifactException::TestArtifactException(const int line, const std::string &msg, const char *filename) {
        std::cout << "Error: " << msg << " at line " << line << " at file " << filename << std::endl;
    }
}

#endif
