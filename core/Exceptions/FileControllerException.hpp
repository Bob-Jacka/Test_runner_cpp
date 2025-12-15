#ifndef FILECONTROLLEREXCEPTION_HPP
#define FILECONTROLLEREXCEPTION_HPP

#include "Base_exception.hpp"

namespace Check_exceptions {
    class FileControllerException final : public BaseException {
    public:
        explicit FileControllerException(const std::string &);
    };

    inline FileControllerException::FileControllerException(const std::string &msg) {
        Utility::message_with_location(msg);
    }
}

#endif
