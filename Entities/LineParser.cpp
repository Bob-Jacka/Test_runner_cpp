#include "LineParser.hpp"

import UtilFuncs_mod;

class File_controller;

Line_interpreter_ns::DirectiveInterpreter::DirectiveInterpreter() {
    this->interpreter_position = 0;
    this->output_vector = std::vector<std::string>();
    this->suit_parameters = std::map<std::string, std::string>();
}

/**
 * Execute logical expression with two arguments and return result of execution.
 * @param parameters line that contains arguments and comparison sign.
 * @return bool value of result, where false means left operand is not equal (less) that right operand.
 */
bool Line_interpreter_ns::DirectiveInterpreter::interpret_logical_expression(const std::string &parameters) const {
    bool result = false;
    const auto arguments = utility::split(parameters); //split arguments to execute them, arguments vector ex. - [a, <, b]
    if (arguments[1] == "<") {
        result = arguments[0] < arguments[2];
    } else if (arguments[1] == ">") {
        result = arguments[0] > arguments[2];
    } else if (arguments[1] == "==") {
        result = arguments[0] == arguments[2];
    } else if (arguments[1] == "!=") {
        result = arguments[0] != arguments[2];
    } else {
        throw Check_exceptions::LineInterpreterException("Unknown parameter form given: " + parameters);
    }
    return result;
}

/**
 * Move interpreter position to the next directive that given
 * @param directive_to_jump name of the directive to jump on
 * @return bool value of directive next position, return true if directive is found, false if directive not found
 */
bool Line_interpreter_ns::DirectiveInterpreter::jmp_to(const std::string &directive_to_jump) const {
    const auto it = std::ranges::find(input_vector_to_proceed, directive_to_jump);
    size_t directive_index = 0;
    const auto current_position_iter = input_vector_to_proceed.begin() + interpreter_position;
    if (it != input_vector_to_proceed.end()) {
        directive_index = std::distance(current_position_iter, it);
    } else {
        return false;
    }

    if (directive_index) {
        interpreter_position = static_cast<int>(directive_index); //set new interpreter position if success
        return true;
    }
    return false;
}

std::map<std::string, std::string> Line_interpreter_ns::DirectiveInterpreter::get_suit_parameters() const {
    return this->suit_parameters;
}

std::vector<std::string> Line_interpreter_ns::DirectiveInterpreter::get_output_vector() const {
    return this->output_vector;
}

/**
 * Increment interpreter position in suit file
 */
void Line_interpreter_ns::DirectiveInterpreter::increment_interpreter_position() const {
    ++this->interpreter_position;
}

/**
 * Add lines to input_vector_to_proceed, till line not starts with another directive symbol
 * @param line_element_start element to use as a predicate in inner loop
 */
void Line_interpreter_ns::DirectiveInterpreter::add_till_line_starts(const std::string &line_element_start) const {
    while (true) {
        if (const auto inner_line = input_vector_to_proceed[this->interpreter_position]; !inner_line.starts_with(line_element_start)) {
            output_vector.push_back(inner_line);
            this->increment_interpreter_position();
        } else {
            break;
        }
    }
}

/**
 * Proceed test cases in group by attaching hash to ts line.
 * @param directive_name name of the directive (its name).
 * @throw LineInterpreterException if line not end with semicolon
*/
void Line_interpreter_ns::DirectiveInterpreter::directive_group(const std::string &directive_name) const {
    if (directive_name.ends_with(group_indicator)) {
        //check for directive ending and process inner test cases in group if they exist
        while (true) {
            if (const auto inner_line = input_vector_to_proceed[this->interpreter_position]; !inner_line.ends_with(suit_directive_end)) {
                output_vector.push_back(utility::hash(inner_line)); //add hash identifier to line and push it
                this->increment_interpreter_position();
            } else {
                break;
            }
        }
    } else {
        throw Check_exceptions::LineInterpreterException("Expected group ending in line with semicolon (:)");
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
            throw Check_exceptions::LineInterpreterException("Suit parameters should contain equal sign (=), but given " + parameter);
        }
    }
    //It can be an error, because no parameters are given, but on the other hand it is just a message and user might not provide any parameters
    if (this->suit_parameters.empty()) {
        utility::println("Expected parameters directive with valid attributes");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * High level method that controls branching operators.
 * @paragraph Example of usage:
 *
 * #Parameters: a=1, b=3
 *
 * #If a == 2:
 *  <some action>
 * #Elif b > 2:
 *  <some action>
 * #Else:
 *  <some else action>
 * @param _arguments arguments that will be given to if, elif or else directives.
 * @throw LineInterpreterException if wrong method usage
 */
bool Line_interpreter_ns::DirectiveInterpreter::high_level_branch_wrapper(const std::string &_arguments) const {
    /**
     * Check for equal operators in given string
     */
    const auto check_func_full = [&](const std::string &to_check) -> bool {
        const auto allowed_operators = std::vector<std::string>{"<", ">", "==", "!="};
        for (const auto &allowed_operator: allowed_operators) {
            if (to_check.contains(allowed_operator)) {
                return true;
            }
        }
        return false;
    };

    /**
     * Lambda func for conditions like just - "true_cond" or "a" (where "a" is the name of variable, given in parameters directive)
     * And if that condition is true, method will include test cases
     */
    const auto check_func_short = [&](const std::string &to_check) -> bool {
        return !to_check.contains("=");
    };

    if (check_func_full(_arguments) or check_func_short(_arguments)) {
        const auto branch_result = interpret_logical_expression(_arguments);
        if (branch_result) {
            add_till_line_starts(directive_start);
            return true;
        } // else branch is not success
        return false;
    } else {
        throw Check_exceptions::LineInterpreterException("Unknown if directive state: " + _arguments);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Import another test suit into main suit
 * @param import_arguments name of the test suit to import. Can be with file extension or without extension.
 * @return vector with strings
 */
std::vector<std::string> Line_interpreter_ns::DirectiveInterpreter::directive_import(const std::string &import_arguments) const {
    const auto imported_lines = File_controller::readlines(import_arguments);
    return imported_lines;
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
void Line_interpreter_ns::DirectiveInterpreter::parse_directives(const std::vector<std::string> &input_vector) const {
    while (interpreter_position != input_vector.size()) {
        input_vector_to_proceed = input_vector;
        if (const auto &line = input_vector_to_proceed[interpreter_position]; line.starts_with(directive_start)) {
            const auto split_string_line = utility::split(line, ' '); ///split directive line, 0 - dir name, 1 - param
            const auto directive_name = split_string_line[0].substr(1, split_string_line[0].length() - 2); //current directive name

            if (directive_name == group_directive_start) {
                directive_group(directive_name);
            } else if (directive_name == if_directive) {
                if (!high_level_branch_wrapper(split_string_line[1])) {
                    if (jmp_to(elif_directive)) {
                        if (!high_level_branch_wrapper(split_string_line[1])) {
                            if (jmp_to(else_directive)) {
                                [[maybe_unused]] high_level_branch_wrapper(split_string_line[1]);
                            } //TODO капец как не уверен в этой лестнице, потом переписать на что то более элегантное
                        }
                    }
                }
            } else if (directive_name == import_directive) {
                for (const auto &enter_line: directive_import(split_string_line[1])) {
                    output_vector.push_back(enter_line); //push lines into this
                }
            } else if (directive_name == parameters_directive) {
                parse_parameters(split_string_line[1]);
            } else {
                throw Check_exceptions::LineInterpreterException("Invalid line directive detected: " + directive_name);
            }
        }
    }
}

/**
 * Delete comment lines from input vector
 * @return lines vector
 */
void Line_interpreter_ns::DirectiveInterpreter::parse_lines_empty(std::vector<std::string> &input_vector) const {
    for (int i = 0; i < input_vector.size(); ++i) {
        if (auto &line = input_vector[i]; line.starts_with(comment)) {
            input_vector.erase(input_vector.begin() + i);
        }
    }
}
