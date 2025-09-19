#ifndef FILECONTROLLEREXCEPTION_HPP
#define FILECONTROLLEREXCEPTION_HPP

#include "Base_exception.hpp"

class FileControllerException final : public BaseException {
public:
    explicit FileControllerException(const std::string &);

    ~FileControllerException() override;
};

inline FileControllerException::FileControllerException(const std::string &msg) {
    utility::message_with_location(msg);
}

#endif
