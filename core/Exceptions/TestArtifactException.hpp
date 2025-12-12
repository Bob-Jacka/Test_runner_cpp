#ifndef TESTARTIFACTEXCEPTION_HPP
#define TESTARTIFACTEXCEPTION_HPP

#include "Base_exception.hpp"
#include <string>

namespace Check_exceptions {
    class TestArtifactException final : public BaseException {
    public:
        explicit TestArtifactException(const std::string &);
    };

    inline TestArtifactException::TestArtifactException(const std::string &msg) {
        Utility::message_with_location(msg);
    }
}

#endif
