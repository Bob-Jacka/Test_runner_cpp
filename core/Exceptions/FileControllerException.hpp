#ifndef FILECONTROLLEREXCEPTION_HPP
#define FILECONTROLLEREXCEPTION_HPP

#include "Base_exception.hpp"

namespace Check_exceptions {
    class FileControllerException final : public BaseException {
    public:
        explicit FileControllerException(int, const char *, const char *);
    };

    inline FileControllerException::FileControllerException(const int line, const char * msg,
                                                            const char *filename) {
        std::cout << "Error: " << msg << " at line " << line << " at file " << filename << std::endl;
    }
}

#endif
