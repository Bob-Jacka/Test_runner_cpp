#include "LineParser.hpp"
#include <stdexcept>
#include "../Exceptions/LineInterpreterException.hpp"

import UtilFuncs_mod;

class File_controller;

/**
 * Import different suit into given suit.
 * @return modified suit strings.
 */
std::vector<std::string> Line_parser::import_suits(File_controller *file_controller) const {
    //
}

Line_parser::Line_parser() {
    //
}

/**
 * Proceed directives in input vector.
 */
void Line_parser::parse_directives() const {
    for (const auto &line: this->main_suit) {
        if (line.starts_with(directive_start)) {
            const std::vector<std::string> split_string_line = utility::split(line, ' '); ///split directive line, 0 - dir name, 1 - param
            const auto cleared_dir_name = split_string_line[0].substr(1, split_string_line[0].length() - 2); //current dir name
            switch (cleared_dir_name) {
                case group_directive_start: //directive for inner group of test cases start
                    if (cleared_dir_name.ends_with(test_group_indicator)) {
                        //
                    } else {
                        throw LineInterpreterException("Expected group ending in line with semicolon (:)");
                    }
                    break;
                case if_directive: //if directive branch
                    break;
                case else_directive: //else directive branch
                    break;
                case import_directive: //import suit into another suit
                    this->main_suit.push_back(import_suits(TODO)); //push lines into this
                    break;
                default:
                    throw std::invalid_argument("Invalid line directive detected: " + cleared_dir_name);
            }
        }
    }
}

/**
 * Delete comment lines from input vector
 * @return lines vector
 */
void Line_parser::parse_lines_empty() const {
    for (const auto &line: this->main_suit) {
        if (line.starts_with(comment)) {
            this->main_suit.erase(this->main_suit.cbegin());
        }
    }
}
