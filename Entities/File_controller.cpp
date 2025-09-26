#include "File_controller.hpp"

/**
 * Open file by given string.
 * @param file_name name of the file to open.
 */
void File_controller::open_file(const std::string &file_name) const {
    this->input_file->open(file_name);
}

/**
 * Method for closing input file.
 */
void File_controller::close_file() const {
    this->input_file->close();
}

/**
 * Read lines from inner open file
 * @return vector with strings (lines)
 */
std::vector<std::string> File_controller::readlines() {
    if (this->input_file->is_open()) {
        auto lines = std::vector<std::string>();
        while (!this->input_file->eof()) {
            std::string line;
            getline(this->input_file, line);
            lines.push_back(line);
        }
        return lines;
    } else {
        throw;
    }
}

/**
 * Read one line from inner open file.
 * @return string (line)
 */
std::string File_controller::readline() {
    if (this->input_file->is_open()) {
        std::string line;
        getline(this->input_file.get(), line);
        return line;
    } else {
        throw;
    }
}

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
        return lines;
    } else {
        throw;
    }
}

/**
 * Static file operation for reading multiple lines.
 * @return vector with strings
 */
std::vector<std::string> File_controller::readlines(std::ofstream &file_descriptor, const int line_count) {
    if (file_descriptor.is_open()) {
        auto lines = std::vector<std::string>(line_count);
        for (int i = 0; i < line_count; ++i) {
            std::string line;
            getline(file_descriptor, line);
            lines.push_back(line);
        }
        return lines;
    } else {
        throw;
    }
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
        return line;
    } else {
        throw;
    }
}

/**
 * Static file operation for reading line.
 * @param file_descriptor file object to read from file.
 * @return line from file.
 */
std::string File_controller::readline(std::ofstream &file_descriptor) {
    //
}

/**
 * Function for creating test result file.
 * @param results_file_name string value for file name.
 * @return file descriptor.
 */
std::fstream File_controller::create_test_result_file(std::string results_file_name) {
    //
}
