#include "LineParser.hpp"

import Libio;

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
    const auto arguments = Utility::split(parameters); //split arguments to execute them, arguments vector ex. - [a, <, b]
    if (arguments[1] == "<") {
        result = arguments[0] < arguments[2];
    } else if (arguments[1] == ">") {
        result = arguments[0] > arguments[2];
    } else if (arguments[1] == "==") {
        result = arguments[0] == arguments[2];
    } else if (arguments[1] == "!=") {
        result = arguments[0] != arguments[2];
    } else {
        throw Check_exceptions::LineInterpreterException(__LINE__, "Unknown parameter form given: " + parameters, __FILE_NAME__);
    }
    return result;
}

/**
 * Move interpreter position to the next directive that given
 * @param directive_to_jump name of the directive to jump on
 * @return bool value of directive next position, return true if directive is found, false if directive not found
 */
bool Line_interpreter_ns::DirectiveInterpreter::jmp_to(const std::string &directive_to_jump) const {
    const auto it = std::ranges::find(inner_vector_to_proceed, directive_to_jump);
    size_t directive_index = 0;
    const auto current_position_iter = inner_vector_to_proceed.begin() + interpreter_position;
    if (it != inner_vector_to_proceed.end()) {
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
 * Proceed test cases in group by attaching hash to ts line.
 * @param directive_args name of the directive (its name).
 * @throw LineInterpreterException if line not end with semicolon
*/
void Line_interpreter_ns::DirectiveInterpreter::directive_group(const std::string &directive_args) const {
    if (directive_args.ends_with(group_indicator)) {
        //check for directive ending and process inner test cases in group if they exist
        while (true) {
            auto inner_line = inner_vector_to_proceed[this->interpreter_position];
            if (!inner_line.contains(suit_directive_end)) {
                //add hash identifier to line and push it
                output_vector.push_back(Utility::hash<1>(libio::string::delete_whitespaces(inner_line), directive_args));
                increment_interpreter_position();
            } else {
                break;
            }
        }
    } else {
        throw Check_exceptions::LineInterpreterException(__LINE__, "Expected group ending in line with semicolon (:), but given " + directive_args,
                                                         __FILE_NAME__);
    }
}

/**
 * Parse parameters directive and insert suit parameters into inner map for later use.
 * @param parameters_line string line to parse for parameters.
 * @throw LineInterpreterException if given parameters are in wrong form.
 */
void Line_interpreter_ns::DirectiveInterpreter::parse_parameters(const std::string &parameters_line) const {
    const auto split_line = Utility::split(parameters_line, ',');
    auto check_lambda = [&](const std::string &to_check) -> bool {
        return to_check.contains("=");
    };

    for (const auto &parameter: split_line) {
        if (check_lambda(parameter)) {
            auto name_and_value = Utility::split(parameter, '=');
            suit_parameters[name_and_value[0]] = name_and_value[1];
        } else {
            throw Check_exceptions::LineInterpreterException(__LINE__, "Suit parameters should contain equal sign (=), but given " + parameter,
                                                             __FILE_NAME__);
        }
    }
    //It can be an error, because no parameters are given, but on the other hand it is just a message and user might not provide any parameters
    if (this->suit_parameters.empty()) {
        Utility::println("Expected parameters directive with valid attributes");
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
 * #End_if
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
        if (interpret_logical_expression(_arguments)) {
            while (true) {
                if (const auto inner_line = inner_vector_to_proceed[this->interpreter_position];
                    !inner_line.starts_with(directive_start_sym) or !inner_line.starts_with(endif_directive)
                ) {
                    output_vector.push_back(inner_line);
                    this->increment_interpreter_position();
                } else {
                    break;
                }
            }
            return true;
        } // else branch is not success
        return false;
    }
    throw Check_exceptions::LineInterpreterException(__LINE__, "Unknown if directive state: " + _arguments, __FILE_NAME__);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

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
std::vector<std::string> Line_interpreter_ns::DirectiveInterpreter::parse_directives(const std::vector<std::string> &input_vector) const {
    inner_vector_to_proceed = input_vector;
    while (interpreter_position != input_vector.size() - 1) {
        if (const auto &line = inner_vector_to_proceed[interpreter_position++]; line.starts_with(directive_start_sym)) {
            const auto split_string_line = Utility::split_by_first_delim(line, ' '); //split directive line, 0 - dir name, 1 - param
            const auto directive_name = split_string_line[0].substr(1, split_string_line[0].length() - 1); //current directive name

            const auto directive_value = libio::string::delete_whitespaces(split_string_line[1]);

            if (directive_value.contains("\"")) {
                throw Check_exceptions::LineInterpreterException(__LINE__, "Value should not contain \" symbol", __FILE_NAME__);
            }

            if (directive_name == group_directive_start) {
                directive_group(directive_value);
            } else if (directive_name == if_directive) {
                if (not high_level_branch_wrapper(directive_value)) {
                    if (jmp_to(elif_directive)) {
                        if (not high_level_branch_wrapper(directive_value)) {
                            if (jmp_to(else_directive)) {
                                high_level_branch_wrapper(directive_value);
                            } //TODO капец как не уверен в этой лестнице, потом переписать на что то более элегантное
                        }
                    }
                }
            } else if (directive_name == import_directive) {
                for (const auto &enter_line: File_controller::readlines(directive_value)) {
                    output_vector.push_back(enter_line); //push lines into this
                }
            } else if (directive_name == parameters_directive) {
                parse_parameters(directive_value);
            } else {
                throw Check_exceptions::LineInterpreterException(__LINE__, "Invalid line directive detected: " + directive_name, __FILE_NAME__);
            }
        }
    }
    return output_vector;
}

/**
 * Delete comment lines from input vector.
 * Also delete ignore lines that starts with -ignore directive
 */
void Line_interpreter_ns::DirectiveInterpreter::parse_lines_empty(std::vector<std::string> &input_vector) const {
    for (int i = 0; i < input_vector.size(); ++i) {
        //TODO не удаляет комментарии
        if (auto &line = input_vector[i]; line.starts_with(comment_sym) or line.starts_with(ignore_directive) or line.empty()) {
            input_vector.erase(input_vector.begin() + i);
        }
    }
}
