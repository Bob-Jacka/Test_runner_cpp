#include "File_controller.hpp"

#include "../Exceptions/FileControllerException.hpp"

/**
 * Static read lines file, open another file and read strings (lines).
 * @return vector with strings (lines)
 */
std::vector<std::string> File_controller::readlines(const std::string &file_name, const int line_count) {
    if (auto file = std::ifstream(file_name); file.is_open()) {
        auto lines = std::vector<std::string>(line_count);
        for (int i = 0; i < line_count; ++i) {
            std::string line;
            getline(file, line);
            lines.push_back(line);
        }
        file.close();
        return lines;
    }
    throw Check_exceptions::FileControllerException("Input file not opened");
}

/**
 * Static function for reading file.
 * Read file till end of the file.
 * @param file_name file name
 * @return vector with lines (strings)
 */
std::vector<std::string> File_controller::readlines(const std::string &file_name) {
    if (auto file = std::ifstream(file_name); file.is_open()) {
        auto lines = std::vector<std::string>();
        std::string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
        return lines;
    }
    throw Check_exceptions::FileControllerException("Input file not opened");
}

/**
 * Static file operation for reading multiple lines.
 * @return vector with strings
 */
std::vector<std::string> File_controller::readlines(std::ifstream &file_descriptor, const int line_count) {
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
    throw Check_exceptions::FileControllerException("Input file not opened");
}

/**
 * Static file operation for reading line.
 * @param file_descriptor name of the file to open.
 * @return line from file.
 */
std::string File_controller::readline(const std::string &file_descriptor) {
    if (auto file = std::ifstream(file_descriptor); file.is_open()) {
        std::string line;
        getline(file, line);
        file.close();
        return line;
    }
    throw Check_exceptions::FileControllerException("Input file not opened");
}

/**
 * Static file operation for reading line.
 * @param file_descriptor file object to read from file.
 * @return line from file.
 */
std::string File_controller::readline(std::ifstream &file_descriptor) {
    if (file_descriptor.is_open()) {
        std::string line;
        getline(file_descriptor, line);
        file_descriptor.close();
        return line;
    }
    throw Check_exceptions::FileControllerException("Input file not opened");
}

/**
 * Function for creating test result file.
 * @param results_file_name string value for file name.
 * @return file descriptor.
 */
std::fstream File_controller::create_test_result_file(const std::string &results_file_name) {
    if (auto file = std::ifstream(results_file_name); file.is_open()) {
        //
    }
}
