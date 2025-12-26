#include "File_controller.hpp"

#include "../Exceptions/FileControllerException.hpp"

/**
* Open file and return condition variable of open
* @param file_name name of the file to open
* @return tuple with file handler and bool (true if file open or error)
*/
std::tuple<std::ifstream, bool> Check_runner::File_controller::open_file(const std::string &file_name) {
    if (auto file = std::ifstream(file_name); file.is_open()) {
        return std::make_tuple<std::ifstream, bool>(std::move(file), true);
    }
    throw Check_exceptions::FileControllerException(__LINE__, "File not exist with name " + file_name, __FILE_NAME__);
}

/**
 * Static read lines file, open another file and read strings (lines).
 * @throw FileControllerException - input file not opened
 * @return vector with strings (lines)
 */
std::vector<std::string> Check_runner::File_controller::readlines(const std::string &file_name, const int line_count) {
    auto [file, cond] = open_file(file_name);
    if (cond) {
        auto lines = std::vector<std::string>(line_count);
        for (int i = 0; i < line_count; ++i) {
            std::string line;
            getline(file, line);
            lines.push_back(line);
        }
        file.close();
        return lines;
    }
    throw Check_exceptions::FileControllerException(__LINE__, "Input file not opened with name with name " + file_name,__FILE_NAME__);
}

/**
 * Static function for reading file.
 * Read file till end of the file.
 * @param file_name file name
 * @throws FileControllerException - input file not opened or FileControllerException - wrong file extension
 * @return vector with lines (strings)
 */
std::vector<std::string> Check_runner::File_controller::readlines(const std::string &file_name) {
    if (check_file_extension(file_name) == 1) {
        auto [file, cond] = open_file(file_name);
        if (cond) {
            auto lines = std::vector<std::string>();
            std::string line;
            while (getline(file, line)) {
                lines.push_back(line);
            }
            file.close();
            return lines;
        }
        throw Check_exceptions::FileControllerException(__LINE__, "Input file not opened with name " + file_name, __FILE_NAME__);
    }
    if (check_file_extension(file_name) == 2) {
        const auto fake_string = file_name + ".txt";
        return readlines(fake_string);
    }
    throw Check_exceptions::FileControllerException(__LINE__, "Wrong file extension " + file_name, __FILE_NAME__);
}

/**
 * Static file operation for reading multiple lines.
 * @throw FileControllerException input file not opened.
 * @return vector with strings
 */
std::vector<std::string> Check_runner::File_controller::readlines(std::ifstream &file_descriptor, const int line_count) {
    if (file_descriptor.is_open()) {
        auto lines = std::vector<std::string>(line_count);
        for (int i = 0; i < line_count; ++i) {
            std::string line;
            getline(file_descriptor, line);
            lines.push_back(line);
        }
        file_descriptor.close();
        return lines;
    }
    throw Check_exceptions::FileControllerException(__LINE__, "Input file not opened with file descriptor", __FILE_NAME__);
}

/**
 * Static file operation for reading line.
 * @param file_descriptor name of the file to open.
 * @throw FileControllerException input file not opened.
 * @return line from file.
 */
std::string Check_runner::File_controller::readline(const std::string &file_descriptor) {
    auto [file, cond] = open_file(file_descriptor);
    if (cond) {
        std::string line;
        getline(file, line);
        file.close();
        return line;
    }
    throw Check_exceptions::FileControllerException(__LINE__, "Input file not opened with string file descriptor " + file_descriptor, __FILE_NAME__);
}

/**
 * Static file operation for reading line.
 * @param file_descriptor file object to read from file.
 * @throw FileControllerException input file not opened.
 * @return line from file.
 */
std::string Check_runner::File_controller::readline(std::ifstream &file_descriptor) {
    if (file_descriptor.is_open()) {
        std::string line;
        getline(file_descriptor, line);
        file_descriptor.close();
        return line;
    }
    throw Check_exceptions::FileControllerException(__LINE__, "Input file not opened with ifstream file descrtiptor", __FILE_NAME__);
}

/**
 * Function for creating test result file.
 * @param results_file_name string value for file name.
 * @throw FileControllerException cannot create test result file.
 * @return file descriptor.
 */
std::fstream Check_runner::File_controller::create_test_result_file(const std::string &results_file_name) {
    auto [file, cond] = open_file(results_file_name);
    if (cond) {
        //
    }
    throw Check_exceptions::FileControllerException(__LINE__, "Cannot create test result file", __FILE_NAME__);
}

/**
* Check for file extension.
* @param file_name name of the file to check.
* @return bool value of checking state.
*/
int Check_runner::File_controller::check_file_extension(const std::string &file_name) {
    if (file_name.ends_with(".txt") and not file_name.ends_with(".")) {
        return 1;
    }
    if (file_name.ends_with("")) {
        return 2;
    }
    return 0;
}

/**
* Function for checking if file exists in filesystem.
* @param file_name name of the file to check.
*/
bool Check_runner::File_controller::check_file_existence(const std::string &file_name) {
    auto [file, cond] = open_file(file_name);
    if (cond) {
        file.close();
        return true;
    }
    file.close();
    return false;
}

#ifdef LINUX
#include <sys/mman.h>

/**
 * Map file with usage of system call.
 * @return array with strings
 */
char* map_file(ifstream &fd) {
    size_t length = 100; // Length to map
    char *mapped = (char *)mmap(nullptr, length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    munmap(mapped, length);
    close(fd);
    return mapped;
}
#endif
