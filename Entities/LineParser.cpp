#include "LineParser.hpp"

#include <algorithm>

#include "../Exceptions/LineInterpreterException.hpp"

import UtilFuncs_mod;

class File_controller;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Proceed test cases in group by attaching hash to ts line.
 * @param main_suit suit with test lines.
 * @param directive_name name of the directive (its name).
 * @throw LineInterpreterException if line not end with semicolon
*/
void Line_interpreter_ns::DirectiveInterpreter::directive_group(std::vector<std::string> &main_suit,
                                                                const std::string &directive_name) const {
    if (directive_name.ends_with(test_group_indicator)) {
        //check for directive ending and process inner test cases in group if they exist
        while (true) {
            if (const auto inner_line = main_suit[this->interpreter_position]; !inner_line.ends_with(suit_directive_end)) {
                const auto hashed_comment_line = utility::hash(inner_line);
                main_suit[this->interpreter_position] = hashed_comment_line;
                ++this->interpreter_position;
            } else {
                break;
            }
        }
    } else {
        throw LineInterpreterException("Expected group ending in line with semicolon (:)");
    }
}

/**
 * Check for given condition in arguments string object.
 * Will add ts below if condition is true and do nothing if false.
 * Arguments example - "1 < 2" or "test_mode" or "dev_mode", arguments should be one string with or without equality operators.
 * @param if_arguments arguments to execute in if directive.
 * @throw LineInterpreterException if unknown condition has met.
 */
void Line_interpreter_ns::DirectiveInterpreter::directive_if(const std::string &if_arguments) const {
    /**
     * Lambda func for conditions like - "x > y", where equility operator exist
     */
    const auto check_func_full = [&](const std::string &to_check) -> bool {
        const auto allowed_operators = std::vector<std::string>{"<", ">", "=="};
        for (auto allowed_operator: allowed_operators) {
            if (to_check.contains(allowed_operator)) {
                return true;
            }
        }
        return false;
    };

    /**
     * Lambda func for conditions like just - "true_cond"
     */
    const auto check_func_short = [&](const std::string &to_check) -> bool {
        //
    };

    //algorithm
    bool result;
    if (check_func_full(if_arguments)) {
        //
    } else if (check_func_short(if_arguments)) {
        //
    } else {
        throw LineInterpreterException("Unknown if directive state: " + if_arguments);
    }
}

/**
 * Check for given condition in arguments string object.
 * Can be placed in suit - only if "if" operator is above
 * @param elif_arguments arguments to execute in elif directive
 * @throw LineInterpreterException will be thrown if no "if" operator exist
 */
void Line_interpreter_ns::DirectiveInterpreter::directive_elif(const std::string &elif_arguments) const {
    //
}

/**
 * Additional directive to if.
 * Will be executed if "if" directive is not true.
 */
void Line_interpreter_ns::DirectiveInterpreter::directive_else() const {
    //
}

/**
 * Import another test suit into main suit
 * @param import_arguments name of the test suit to import. Can be with file extension or without extension.
 */
std::vector<std::string> Line_interpreter_ns::DirectiveInterpreter::directive_import(const std::string &import_arguments) const {
    //
    return;
}

int Line_interpreter_ns::DirectiveInterpreter::get_interpreter_position() const {
    return this->interpreter_position;
}

void Line_interpreter_ns::DirectiveInterpreter::set_interpreter_position(const int &interpreter_position) {
    this->interpreter_position = interpreter_position;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Private function for import different suit into given suit.
 * @return modified suit strings.
 */
std::vector<std::string> Line_parser::import_suits(File_controller *file_controller) const {
    //
}

Line_parser::Line_parser() {
    this->interpreter = Line_interpreter_ns::DirectiveInterpreter();
}

Line_parser::~Line_parser() {
    delete &this->interpreter;
}

std::vector<std::string> Line_parser::get_main_suit() const {
    return this->main_suit;
}

void Line_parser::set_main_suits(const std::vector<std::string> &main_suits) {
    this->main_suit = main_suits;
}

/**
 * Public entry point to line parser.
 * Proceed directives in input vector object one by one.
 * @throw LineInterpreterException if group directive is not valid ending.
 */
void Line_parser::parse_directives() const {
    for (int i = 0; i < this->main_suit.size(); ++i) {
        const auto &line = this->main_suit[i];
        if (line.starts_with(directive_start)) {
            const auto split_string_line = utility::split(line, ' '); ///split directive line, 0 - dir name, 1 - param
            const auto directive_name = split_string_line[0].substr(1, split_string_line[0].length() - 2); //current dir name
            switch (directive_name) {
                case group_directive_start: //directive for inner group of test cases start
                    this->interpreter.directive_group(this->main_suit, directive_name, i);
                    break;
                case if_directive: //if directive branch
                    this->interpreter.directive_if(split_string_line);
                    break;
                case else_directive: //else directive branch
                    this->interpreter.directive_else(split_string_line);
                    break;
                case import_directive: //import suit into another suit
                    this->main_suit.push_back(this->interpreter.directive_import(split_string_line)); //push lines into this
                    break;
                default:
                    throw LineInterpreterException("Invalid line directive detected: " + directive_name);
            }
        }
    }
}

/**
 * Delete comment lines from input vector
 * @return lines vector
 */
void Line_parser::parse_lines_empty() const {
    for (int i = 0; i < this->main_suit.size(); ++i) {
        auto line = this->main_suit[i];
        if (line.starts_with(comment)) {
            this->main_suit.erase(std::vector<auto>::reference(this->main_suit.begin() + i));
        }
    }
}
