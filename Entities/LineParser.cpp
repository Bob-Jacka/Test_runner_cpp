#include "LineParser.hpp"

#include "../Exceptions/LineInterpreterException.hpp"

import UtilFuncs_mod;

class File_controller;

/**
 * Execute logical expression with two arguments.
 * @param parameters line that contains arguments and comparison sign.
 * @return bool value of result
 */
bool Line_interpreter_ns::DirectiveInterpreter::interpret_logical_expression(const std::string &parameters) const {
    bool result = false;
    const auto arguments = utility::split(parameters); //split arguments to execute them
    if (arguments[1] == "<") {
        result = arguments[0] < arguments[2];
    } else if (arguments[1] == ">") {
        result = arguments[0] > arguments[2];
    } else if (arguments[1] == "==") {
        result = arguments[0] == arguments[2];
    } else if (arguments[1] == "!=") {
        result = arguments[0] != arguments[2];
    } else {
        throw LineInterpreterException("Unknown parameter form: " + parameters);
    }
    return result;
}

Line_interpreter_ns::DirectiveInterpreter::DirectiveInterpreter() {
    this->interpreter_position = 0;
    this->suit_parameters = std::map<std::string, std::string>();
}

std::map<std::string, std::string> Line_interpreter_ns::DirectiveInterpreter::get_suit_parameters() const {
    return this->suit_parameters;
}

/**
 * Increment interpreter position in suit file
 */
void Line_interpreter_ns::DirectiveInterpreter::increment_interpreter_position() const {
    ++this->interpreter_position;
}

/**
 * Add lines to main_suit, till line not starts with another directive symbol
 * @param line_element_start element to use as a predicate in inner loop
 */
void Line_interpreter_ns::DirectiveInterpreter::add_till_line_starts(const std::string &line_element_start) const {
    while (true) {
        if (const auto inner_line = main_suit[this->interpreter_position]; !inner_line.starts_with(line_element_start)) {
            main_suit[this->interpreter_position] = inner_line;
            this->increment_interpreter_position();
        } else {
            break;
        }
    }
}

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
                main_suit[this->interpreter_position] = utility::hash(inner_line); //add hash identifier to line
                this->increment_interpreter_position();
            } else {
                break;
            }
        }
    } else {
        throw LineInterpreterException("Expected group ending in line with semicolon (:)");
    }
}

/**
 * Parse parameters directive and insert suit parameters into inner map for later use.
 * @param parameters_line string line to parse for parameters.
 * @throw LineInterpreterException if given parameters are in wrong form.
 */
void Line_interpreter_ns::DirectiveInterpreter::parse_parameters(const std::string &parameters_line) const {
    const auto split_line = utility::split(parameters_line, ',');
    auto check_lambda = [&](const std::string &to_check) -> bool {
        return to_check.contains("=");
    };

    for (const auto &parameter: split_line) {
        if (check_lambda(parameter)) {
            const auto name_and_value = utility::split(parameter, '=');
            this->suit_parameters.insert(name_and_value[0], name_and_value[1]); //insert name and value of parameter into map
        } else {
            throw LineInterpreterException("Suit parameters should contain equal sign (=), but given " + parameter);
        }
    }
    //It can be an error, because no parameters are given, but on the other hand it is just a message and user might not provide any parameters
    if (this->suit_parameters.empty()) {
        utility::println("Expected parameters directive with valid attributes");
    }
}

/**
 * Check for given condition in arguments string object.
 * Will add ts below if condition is true and do nothing if false.
 * Arguments example - "1 < 2" or "test_mode" or "dev_mode", arguments should be one string with or without equality operators.
 * Example of directive usage:
 * #If 1 < 2
 *      test on true condition|High|will add this test if (1 < 2)
 * @param if_arguments arguments to execute in if directive.
 * @throw LineInterpreterException if unknown condition has met.
 */
void Line_interpreter_ns::DirectiveInterpreter::directive_if(const std::string &if_arguments) const {
    /**
     * Lambda func for conditions like - "x > y", where equility operator exist
     */
    const auto check_func_full = [&](const std::string &to_check) -> bool {
        const auto allowed_operators = std::vector<std::string>{"<", ">", "==", "!="};
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
        return !to_check.contains("=");
    };

    //algorithm next
    if (check_func_full(if_arguments)) {
        const auto result = interpret_logical_expression(if_arguments);
        if (result) {
            //add next test cases
        }
    } else if (check_func_short(if_arguments)) {
        //
        add_till_line_starts(directive_start);
    } else {
        throw LineInterpreterException("Unknown if directive state: " + if_arguments);
    }
}

/**
 * Check for given condition in arguments string object.
 * Can be placed in suit - only if "if" operator is above
 * @paragraph Example of usage:
 * #If 1 < 2:
 *  <some action>
 * #Elif true:
 *  <some action>
 * @param elif_arguments arguments to execute in elif directive
 * @throw LineInterpreterException will be thrown if no "elif" operator exist
 */
void Line_interpreter_ns::DirectiveInterpreter::directive_elif(const std::string &elif_arguments) const {
    //
}

/**
 * Additional directive to if.
 * @paragraph Example of usage:
 * #If 1 < 2:
 *  <some action>
 * #Elif 1 > 2:
 *  <some action>
 * #Else:
 *  <some else action>
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

void Line_interpreter_ns::DirectiveInterpreter::set_interpreter_position(const int &interpreter_position) const {
    this->interpreter_position = interpreter_position;
}

/**
 * Public entry point to line parser.
 * Proceed directives in input vector object one by one.
 * @throw LineInterpreterException if group directive is not valid ending.
 */
void Line_interpreter_ns::DirectiveInterpreter::parse_directives() const {
    for (int i = 0; i < this->main_suit.size(); ++i) {
        if (const auto &line = this->main_suit[i]; line.starts_with(directive_start)) {
            const auto split_string_line = utility::split(line, ' '); ///split directive line, 0 - dir name, 1 - param
            const auto directive_name = split_string_line[0].substr(1, split_string_line[0].length() - 2); //current dir name

            if (directive_name == group_directive_start) {
                directive_group(this->main_suit, directive_name);
            } else if (directive_name == if_directive) {
                directive_if(split_string_line[1]);
            } else if (directive_name == elif_directive) {
                directive_elif(split_string_line[1]);
            } else if (directive_name == else_directive) {
                directive_else();
            } else if (directive_name == import_directive) {
                this->main_suit.push_back(this->interpreter.directive_import(split_string_line)); //push lines into this
            } else if (directive_name == parameters_directive) {
                parse_parameters(split_string_line[1]);
            } else {
                throw LineInterpreterException("Invalid line directive detected: " + directive_name);
            }
        }
    }
}

std::vector<std::string> Line_interpreter_ns::DirectiveInterpreter::get_main_suit() const {
    return this->main_suit;
}

void Line_interpreter_ns::DirectiveInterpreter::set_main_suits(const std::vector<std::string> &main_suits) {
    this->main_suit = main_suits;
}

/**
 * Delete comment lines from input vector
 * @return lines vector
 */
void Line_interpreter_ns::DirectiveInterpreter::parse_lines_empty() const {
    for (int i = 0; i < this->main_suit.size(); ++i) {
        if (auto &line = this->main_suit[i]; line.starts_with(comment)) {
            this->main_suit.erase(std::vector<auto>::reference(this->main_suit.begin() + i));
        }
    }
}
