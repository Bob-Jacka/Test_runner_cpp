#include "File_controller.hpp"

#ifdef NEW_FILE_FORMAT
#pragma message ("Using New file format")

std::tuple<std::ifstream, bool> Check_runner::File_controller::create_tf_file(const std::string &file_name) {
    if (auto file = std::ifstream(file_name); file.is_open()) {
        return std::make_tuple<std::ifstream, bool>(std::move(file), true);
    }
    throw Check_exceptions::FileControllerException(__LINE__, "Cannot create tf file", __FILE_NAME__);
}

std::tuple<std::ifstream, bool> Check_runner::File_controller::create_rsf_file(const std::string &file_name) {
    if (auto file = std::ifstream(file_name); file.is_open()) {
        return std::make_tuple<std::ifstream, bool>(std::move(file), true);
    }
    throw Check_exceptions::FileControllerException(__LINE__, "Cannot create rsf file", __FILE_NAME__);
}
#elifndef NEW_FILE_FORMAT
/**
 * Function for creating test result file.
 * @param results_file_name string value for file name.
 * @throw FileControllerException cannot create test result file.
 * @return file descriptor.
 */
std::fstream Check_runner::File_controller::create_test_result_file(const std::string &results_file_name) {
    if (auto [file, cond] = open_file<std::fstream>(results_file_name); cond) {
        return file;
    }
    throw Check_exceptions::FileControllerException(__LINE__, "Cannot create test result file", __FILE_NAME__);
}
#endif

/**
 * Static read lines file, open another file and read strings (lines).
 * @throw FileControllerException - input file not opened
 * @return vector with strings (lines)
 */
std::vector<std::string> Check_runner::File_controller::readlines(const std::string &file_name, const int line_count) {
    auto [file, cond] = open_file<std::ifstream>(file_name);
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
    throw Check_exceptions::FileControllerException(__LINE__, ("Input file not opened with name with name " + file_name).c_str(),
                                                    __FILE_NAME__);
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
        auto [file, cond] = open_file<std::ifstream>(file_name);
        if (cond) {
            auto        lines = std::vector<std::string>();
            std::string line;
            while (getline(file, line)) {
                lines.push_back(line);
            }
            file.close();
            return lines;
        }
        throw Check_exceptions::FileControllerException(__LINE__, ("Input file not opened with name " + file_name).c_str(),
                                                        __FILE_NAME__);
    }
    if (check_file_extension(file_name) == 2) {
        const auto fake_string = file_name + ".txt";
        return readlines(fake_string);
    }
    throw Check_exceptions::FileControllerException(__LINE__, ("Wrong file extension " + file_name).c_str(), __FILE_NAME__);
}

/**
 * Static file operation for reading multiple lines.
 * @throw FileControllerException input file not opened.
 * @return vector with strings
 */
std::vector<std::string>
Check_runner::File_controller::readlines(std::ifstream &file_descriptor, const int line_count) {
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
    throw Check_exceptions::FileControllerException(__LINE__, "Input file not opened with file descriptor",
                                                    __FILE_NAME__);
}

/**
 * Static file operation for reading line.
 * @param file_descriptor name of the file to open.
 * @throw FileControllerException input file not opened.
 * @return line from file.
 */
std::string Check_runner::File_controller::readline(const std::string &file_descriptor) {
    auto [file, cond] = open_file<std::ifstream>(file_descriptor);
    if (cond) {
        std::string line;
        getline(file, line);
        file.close();
        return line;
    }
    throw Check_exceptions::FileControllerException(__LINE__, ("Input file not opened with string file descriptor " +
                                                               file_descriptor).c_str(), __FILE_NAME__);
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
    throw Check_exceptions::FileControllerException(__LINE__, "Input file not opened with ifstream file descrtiptor",
                                                    __FILE_NAME__);
}

/**
* Check for file extension.
* @param file_name name of the file to check.
* @return bool value of checking state (1 - if txt file, 2 - empty ext, 3 - tf or rsf, otherwise 0).
*/
int Check_runner::File_controller::check_file_extension(const std::string &file_name) {
    if (file_name.ends_with(TXT_F) and not file_name.ends_with(".")) {
        return 1;
    }
    if (file_name.ends_with("")) [[likely]] {
        return 2;
    }
#ifdef NEW_FILE_FORMAT
    if (file_name.ends_with(RSF_F) or file_name.ends_with(TF)) [[likely]] {
        return 3;
    }
#endif
    return 0;
}

/**
* Function for checking if file exists in filesystem.
* @param file_name name of the file to check.
* @return bool value of file existance
*/
bool Check_runner::File_controller::check_file_existence(std::string &file_name) {
    //check for file extension existence
#ifndef NEW_FILE_FORMAT
    if (check_file_extension(file_name) == 2) {
        //if true - user provided ext //else - user does not provide ext and need to append ext for correct utility work
        file_name += TXT_F;
    }
#endif
    auto [file, cond] = open_file<std::ifstream>(file_name);
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
