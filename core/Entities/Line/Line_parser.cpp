#include "../Line/Line_parser.hpp"

class File_controller;

/**
 * Increment interpreter position
 */
#define INCR_POS ++this->interpreter_position;

/**
 * Get current interpreter position
 */
#define GET_POS this->interpreter_position

#define GLOBAL_PARAMS this->global_parameters

Interpreter_ns::Directive_interpreter::Directive_interpreter() {
    this->interpreter_position    = 0; //start position in interpreter
    this->output_vector           = new std::vector<std::string>();
    this->inner_vector_to_proceed = std::vector<std::string>();
}

/**
 * Execute logical expression with two arguments and return result of execution.
 * @param parameters_line line that contains arguments and comparison sign.
 * @return bool value of result, where false means left operand is not equal (less) that right operand.
 */
bool Interpreter_ns::Directive_interpreter::interpret_logical_expression(const std::string &parameters_line) {
    bool               result    = false;
    const auto         arguments = libio::string::split(parameters_line, " "); //split arguments to execute them, arguments vector ex. - [a, <, b]
    const std::string &op_1      = arguments[0];                               //left operand
    const std::string &operat    = arguments[1];                               //sentence operator
    const std::string &op_2      = arguments[2];                               //right operand
    if (operat == "<") [[unlikely]] {
        result = op_1 < op_2;
    } else if (operat == ">") [[unlikely]] {
        result = op_1 > op_2;
    } else if (operat == "==") [[likely]] {
        result = op_1 == op_2;
    } else if (operat == "!=") {
        result = op_1 != op_2;
    } else {
        throw Check_exceptions::LineInterpreterException(__LINE__, ("Unknown parameter form given: " + parameters_line).c_str(),
                                                         __FILE_NAME__);
    } //or return false
    return result;
}

/**
 * Parse global functions from text.
 * Example: #If Contains(a, b):
 *  ...
 *  Global function should start with uppercase letter.
 */
void Interpreter_ns::Directive_interpreter::parse_global_func() {
}

/**
 * Move interpreter position to the next directive that given
 * @param directive_to_jump name of the directive to jump on
 * @return bool value of directive next position, return true if directive is found, false if directive not found
 */
bool Interpreter_ns::Directive_interpreter::jmp_to(const std::string &directive_to_jump) {
    const auto it                    = std::ranges::find(inner_vector_to_proceed, directive_to_jump);
    size_t     directive_index       = 0;
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

#ifdef GLOBAL_MAPS
/**
 * Method for receiving global parameters
 * @return map with key and value for global parameters
 */
std::map<std::string, std::string> Interpreter_ns::Directive_interpreter::get_suit_parameters() const {

}
#endif

std::vector<std::string> Interpreter_ns::Directive_interpreter::get_output_vector() const {
    return *this->output_vector;
}

/**
 * Proceed test cases in group by attaching hash to ts line.
 * @param directive_args name of the directive (its name).
 * @throw LineInterpreterException if line not end with semicolon
*/
void Interpreter_ns::Directive_interpreter::directive_group(const std::string &directive_args) {
    using High_str = std::string;
    if (directive_args.ends_with(group_indicator)) {
        //check for directive ending and process inner test cases in group if they exist
        High_str hashed;
        while (true) {
            auto inner_line = inner_vector_to_proceed[this->interpreter_position];
            if (!inner_line.contains(suit_directive_end)) {
                //add hash identifier to line and push it
#ifdef DEBUG
                auto cleared = libio::string::delete_whitespaces(inner_line);
                auto hash = Utility::hash<4>(directive_args);
                hashed = hash + cleared;
#elifndef DEBUG
                hashed = Utility::hash<4>(directive_args) + libio::string::delete_whitespaces(inner_line);
#endif
                output_vector->push_back(hashed);
                ++this->interpreter_position;
            } else {
                break;
            }
        }
    } else {
        throw Check_exceptions::LineInterpreterException(__LINE__,
                                                         ("Expected group ending in line with semicolon (:), but given " +
                                                          directive_args).c_str(),
                                                         __FILE_NAME__);
    }
}

/**
 * Parse parameters directive and insert suit parameters into inner map for later use.
 * @param parameters_line string line to parse for parameters.
 * @throw LineInterpreterException if given parameters are in wrong form.
 */
void Interpreter_ns::Directive_interpreter::parse_parameters(const std::string &parameters_line) const {
    using namespace libio::string;
    auto check_lambda = [&](const std::string &to_check) -> bool {
        return to_check.contains("=");
    };
    auto split_line = split(parameters_line, ",");
    if (split_line[0].contains(group_indicator)) {
        split_line[0] = delete_whitespaces(replace_string_all(split_line[0], ":", ""));
    } else if (split_line[1].contains(group_indicator)) {
        split_line[1] = delete_whitespaces(replace_string_all(split_line[1], ":", ""));
    }

    for (const auto &parameter: split_line) {
        if (check_lambda(parameter)) {
#ifdef GLOBAL_MAPS
            auto name_and_value = Utility::split(parameter, '=');
            global_parameters->insert({name_and_value[0], name_and_value[1]}); //attach name to value in global parameters
#endif
        } else {
            throw Check_exceptions::LineInterpreterException(__LINE__,
                                                             ("Suit parameters should contain equal sign (=), but given " +
                                                              parameter).c_str(),
                                                             __FILE_NAME__);
        }
    }
#ifdef GLOBAL_MAPS
    //It can be an error, because no parameters are given, but on the other hand it is just a message and user might not provide any parameters
    if (this->global_parameters->empty()) {
        libio::output::println("Expected parameters directive with valid attributes");
    }
#endif
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
bool Interpreter_ns::Directive_interpreter::high_level_branch_wrapper(const std::string &_arguments) {
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
                    !inner_line.starts_with(directive_start_sym)
#ifdef EXTENDED_FUNCTIONALITY_IF
                    or !inner_line.starts_with(endif_directive)
#endif
                ) {
                    output_vector->push_back(inner_line);
                    ++this->interpreter_position;
                } else {
                    break;
                }
            }
            return true;
        } // else branch is not success
        return false;
    }
    throw Check_exceptions::LineInterpreterException(__LINE__, ("Unknown if directive state: " + _arguments).c_str(),
                                                     __FILE_NAME__);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Interpreter_ns::Directive_interpreter::set_interpreter_position(const int &interpreter_position) {
    this->interpreter_position = interpreter_position;
}

/**
 * Public entry point to line parser.
 * Proceed directives in input vector object one by one.
 * @throw LineInterpreterException if group directive is not valid ending.
 * @return vector with parsed lines
 */
std::vector<std::string>
Interpreter_ns::Directive_interpreter::exec(const std::vector<std::string> &input_vector) {
    inner_vector_to_proceed = input_vector;
    include_cycle(); //first wave of execution
    while (interpreter_position != input_vector.size() - 1) {
        const auto &line = inner_vector_to_proceed[interpreter_position++];
        if (line.starts_with(directive_start_sym)) {
            const auto split_string_line = libio::string::split_by_first_delim(line, ' '); //split directive line, 0 - dir name, 1 - param
            const auto directive_name    = split_string_line[0].substr(1, split_string_line[0].length() - 1); //current directive name
            const auto directive_value   = libio::string::delete_whitespaces(split_string_line[1]); //current directive value

            if (directive_value.contains("\"")) {
                throw Check_exceptions::LineInterpreterException(__LINE__, "Value should not contain \" symbol",
                                                                 __FILE_NAME__);
            }

            if (directive_name == group_directive_start) [[likely]] {
                directive_group(directive_value);
            }
#ifdef EXTENDED_FUNCTIONALITY_IF
                else if (directive_name == if_directive) [[likely]] {
                    if (not high_level_branch_wrapper(directive_value)) {
                        if (jmp_to(elif_directive)) {
                            if (not high_level_branch_wrapper(directive_value)) {
                                if (jmp_to(else_directive)) {
                                    high_level_branch_wrapper(directive_value);
                                }
                            }
                        }
                    }
                }
#endif
            else if (directive_name == import_directive) [[likely]] {
                add_to_output_vector(Check_runner::File_controller::readlines(directive_value));
            } else if (directive_name == parameters_directive) {
                parse_parameters(directive_value);
            } else {
                throw Check_exceptions::LineInterpreterException(__LINE__,
                                                                 ("Invalid line directive detected: " + directive_name).c_str(),
                                                                 __FILE_NAME__);
            }
        }
    }
    return *output_vector;
}

/**
 * Delete comment lines from input vector.
 * Also delete ignore lines that starts with - ignore directive
 * @return vector with lines
 */
std::vector<std::string>
Interpreter_ns::Directive_interpreter::delete_comments(std::vector<std::string> &input_vector) const {
    for (int i = 0; i < input_vector.size();) {
        const auto line = libio::string::delete_whitespaces(input_vector[i]);
        if (line.starts_with(comment_sym) or line.starts_with(ignore_directive) or line.empty()) {
            input_vector.erase(input_vector.begin() + i);
        } else {
            ++i;
        }
    }
    input_vector.shrink_to_fit();
    add_to_output_vector(input_vector);
    return *output_vector;
}

/**
 * Simply adds all elements from input vector to inner output
 * @param input_vector source vector with strings
 */
void Interpreter_ns::Directive_interpreter::add_to_output_vector(const std::vector<std::string> &input_vector) const {
    for (auto &input_elem: input_vector) {
        output_vector->push_back(input_elem);
    }
}

/**
 * Include all files into.
 * First cycle in processing directives
 */
void Interpreter_ns::Directive_interpreter::include_cycle() {
    // const auto size = inner_vector_to_proceed.size() - 1;
    // const auto start_position = interpreter_position;
    // while (interpreter_position != size) {
    //     //
    // }
    // interpreter_position = start_position; //return back to begin
}
