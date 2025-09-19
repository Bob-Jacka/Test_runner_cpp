#ifndef MAINEXCEPTION_HPP
#define MAINEXCEPTION_HPP

#include "Base_exception.hpp"

class MainException final : public BaseException {
public:
    explicit MainException(const std::string &);

    ~MainException() override;
};

inline MainException::MainException(const std::string &msg) {
    utility::message_with_location(msg);
}

#endif
