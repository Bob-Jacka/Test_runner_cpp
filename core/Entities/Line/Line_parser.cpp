#include "../Line/Line_parser.hpp"

/**
 * Increment interpreter position
 */
#define INCR_POS ++this->parser_position;
#define DECR_POS --this->parser_position;

/**
 * Get current interpreter position
 */
#define GET_POS this->parser_position

#define GLOBAL_PARAMS this->global_map

Interpreter_ns::Directive_parser::Directive_parser() {
    this->parser_position         = 0; //start position in interpreter
    this->inner_vector_to_proceed = std::vector<std::string>();
    this->global_map              = new Global();
}

/**
 * Execute logical expression with two arguments and return result of execution.
 * @param parameters_line line that contains arguments and comparison sign.
 * @return bool value of result, where false means left operand is not equal (less) that right operand.
 */
bool Interpreter_ns::Directive_parser::interpret_logical_expression(const std::string &parameters_line) const {
    const auto         arguments = libio::string::split(parameters_line, " ");
    std::string        op_1      = arguments[0];
    const std::string &operat    = arguments[1];
    std::string        op_2      = arguments[2];

    //if map already contains values:
    {
        if (global_map->contains_global(op_1)) {
            op_1 = global_map->get_global_parameter(op_1);
        }
        if (global_map->contains_global(op_2)) {
            op_2 = global_map->get_global_parameter(op_2);
        }
    }

    bool result = false;
    if (operat == "<") [[unlikely]] {
        result = op_1 < op_2;
    } else if (operat == ">") [[unlikely]] {
        result = op_1 > op_2;
    } else if (operat == "==") [[likely]] {
        result = op_1 == op_2;
    } else if (operat == "!=") {
        result = op_1 != op_2;
    } else {
        throw Check_exceptions::LineParserException(__LINE__,
                                                    ("Unknown parameter form given: " + parameters_line).c_str(),
                                                    __FILE_NAME__);
    }
    return result;
}

/**
 * Parse global functions from text.
 * Example: #If Contains(a, b):
 *  ...
 *  Global function should start with uppercase letter.
 */
void Interpreter_ns::Directive_parser::parse_global_func() {
}

/**
 * Move interpreter position to the next directive that given
 * @param directive_to_jump name of the directive to jump on
 * @return bool value of directive next position, return true if directive is found, false if directive not found
 */
bool Interpreter_ns::Directive_parser::jmp_to(const std::string &directive_to_jump) {
    for (int i = parser_position; i < inner_vector_to_proceed.size(); ++i) {
        if (inner_vector_to_proceed[i].contains(directive_to_jump)) {
            parser_position = i;
            return true;
        }
    }
    return false;
}

std::vector<std::string> Interpreter_ns::Directive_parser::get_output_vector() const {
    return this->inner_vector_to_proceed;
}

/**
 * Proceed test cases in group by attaching hash to ts line.
 * @param directive_args name of the directive (its name).
 * @throw LineInterpreterException if line not end with semicolon
*/
void Interpreter_ns::Directive_parser::directive_group(const std::string &directive_args) {
    auto copy_dir_args  = directive_args;
    int  group_position = 0;
    if (directive_args.ends_with(group_indicator)) {
        DECR_POS
        inner_vector_to_proceed.erase(inner_vector_to_proceed.begin() + parser_position);

        //check for directive ending and process inner test cases in group if they exist
        while (true) {
            auto inner_line = inner_vector_to_proceed[this->parser_position]; //save line to proceed
            if (!inner_line.contains(suit_directive_end)) {
                if (parser_position < inner_vector_to_proceed.size()) {
                    inner_vector_to_proceed.erase(inner_vector_to_proceed.begin() + parser_position);
                }
                std::string hashed = (Utility::hash<4>(directive_args)) + (libio::string::delete_whitespaces(inner_line));
                inner_vector_to_proceed.push_back(hashed); //TODO Add extra string to vector
            } else {
                inner_vector_to_proceed.erase(inner_vector_to_proceed.begin() + parser_position);
                break;
            }
        }
        INCR_POS
    } else {
        throw Check_exceptions::LineParserException(__LINE__,
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
void Interpreter_ns::Directive_parser::parse_parameters(const std::string &parameters_line) const {
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
            auto name_and_value = split(parameter, "=");
            global_map->add_global_parameters(name_and_value[0], name_and_value[1]); //attach name to value in global parameters
        } else {
            throw Check_exceptions::LineParserException(__LINE__,
                                                        ("Suit parameters should contain equal sign (=), but given " +
                                                         parameter).c_str(),
                                                        __FILE_NAME__);
        }
    }
    //It can be an error, because no parameters are given, but on the other hand it is just a message and user might not provide any parameters
    if (this->global_map->empty()) {
        libio::output::println("Expected parameters directive with valid attributes");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool Interpreter_ns::Directive_parser::process_block_until_next() {
    int nesting_level = 1;

    while (parser_position < inner_vector_to_proceed.size()) {
        const auto &line = inner_vector_to_proceed[parser_position];

        if (line.starts_with(directive_start_sym)) {
            auto  split_line                        = libio::string::split_by_first_delim(line, ' ');
            auto &[directive_name, directive_value] = split_line;

            if (directive_name == if_directive) {
                ++nesting_level;
            } else if (directive_name == endif_directive) {
                --nesting_level;
                if (nesting_level == 0) {
                    return false;
                }
            } else if (nesting_level == 1 &&
                       (directive_name == elif_directive || directive_name == else_directive)) {
                return true;
            }
        }

        ++parser_position;
    }

    throw Check_exceptions::LineParserException(__LINE__,
                                                "Missing #End_if directive", __FILE_NAME__);
}

/**
 * High level method that controls branching operators.
 * @paragraph Example of usage:
 *
 * #Parameters: a=1, b=3
 *
 * @code
 * #If a == 2:
 *  <some action>
 * #Elif b > 2:
 *  <some action>
 * #Else:
 *  <some else action>
 * #End_if
 * @endcode
 * @param _arguments arguments that will be given to if, elif or else directives.
 * @throw LineInterpreterException if wrong method usage
 */
bool Interpreter_ns::Directive_parser::high_level_branch_wrapper(const std::string &_arguments) {
    if (interpret_logical_expression(_arguments)) {
        // if true - proceed until
        process_block_until_next();
        skip_to_endif();
        return true;
    }
    skip_to_next_branch();
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Interpreter_ns::Directive_parser::set_parser_position(const int &parser_position) {
    this->parser_position = parser_position;
}

/**
 * Public entry point to line parser.
 * Proceed directives in input vector object one by one.
 * @throw LineInterpreterException if group directive is not valid ending.
 * @return vector with parsed lines
 */
std::vector<std::string>
Interpreter_ns::Directive_parser::exec(const std::vector<std::string> &input_vector) {
    inner_vector_to_proceed = input_vector;
    include_cycle(); //first wave of execution

    //Second wave of execution, proceed other directives
    while (parser_position != inner_vector_to_proceed.size() - 1) {
        const auto &line = inner_vector_to_proceed[parser_position++];
        if (Utility::check_directive(line)) {
            const auto split_string_line = libio::string::split_by_first_delim(line, ' '); //split directive line, 0 - dir name, 1 - param
            auto &     [directive_name, directive_value] = split_string_line;

            //TODO maybe delete this check due to unuseful error throwing
            if (directive_value.contains("\"")) {
                throw Check_exceptions::LineParserException(__LINE__, "Value should not contain \" symbol",
                                                            __FILE_NAME__);
            }

            if (directive_name.substr(1) == group_directive_start) [[likely]] {
                //for groups
                directive_group(directive_value);
            } else if (directive_name.substr(1) == if_directive) [[likely]] {
                //for branches
                high_level_branch_wrapper(directive_value);
            } else if (directive_name.substr(1) == parameters_directive) {
                //for parameters
                parse_parameters(directive_value);
            } else {
                throw Check_exceptions::LineParserException(__LINE__,
                                                            ("Invalid line directive detected: " + directive_name).c_str(),
                                                            __FILE_NAME__);
            }
        }
    }
    return inner_vector_to_proceed;
}

/**
 * Delete comment lines from input vector.
 * Also delete ignore lines that starts with - ignore directive
 * @return vector with lines
 */
std::vector<std::string>
Interpreter_ns::Directive_parser::delete_comments(std::vector<std::string> &input_vector) const {
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
    return inner_vector_to_proceed;
}

/**
 * Simply adds all elements from input vector to inner output
 * @param input_vector source vector with strings
 */
void Interpreter_ns::Directive_parser::add_to_output_vector(const std::vector<std::string> &input_vector) const {
    for (auto &input_elem: input_vector) {
        inner_vector_to_proceed.push_back(input_elem);
    }
}

/**
 * Include all files into.
 * First cycle in processing directives
 */
void Interpreter_ns::Directive_parser::include_cycle() {
    parser_position = 0;
    std::vector<std::string> processed;

    while (parser_position < inner_vector_to_proceed.size()) {
        const auto &line = inner_vector_to_proceed[parser_position];

        if (Utility::check_directive(line)) {
            auto  split_line                        = libio::string::split_by_first_delim(line, ' ');
            auto &[directive_name, directive_value] = split_line;

            if (directive_name == import_directive) {
                std::string file_path      = libio::string::delete_whitespaces(directive_value);
                auto        imported_lines = Check_runner::File_controller::readlines(file_path);
                for (const auto &imported: imported_lines) {
                    processed.push_back(imported);
                }
                ++parser_position;
                continue;
            }
        }
        processed.push_back(line); //if no directive found than test case
        ++parser_position;
    }

    inner_vector_to_proceed = std::move(processed);
    parser_position         = 0;
}

///increase until next branching
void Interpreter_ns::Directive_parser::skip_to_next_branch() {
    int nesting = 1; // inside #If
    while (parser_position < inner_vector_to_proceed.size() && nesting > 0) {
        const auto &line = inner_vector_to_proceed[parser_position];
        if (line.contains(if_directive)) {
            ++nesting;
        } else if (line.contains(endif_directive)) {
            --nesting;
        }
        ++parser_position;
    }
}

/// skip all lines until #End_if
void Interpreter_ns::Directive_parser::skip_to_endif() {
    int        nesting = 1;
    const auto size    = inner_vector_to_proceed.size();
    while (parser_position < size) {
        const auto &line = inner_vector_to_proceed[parser_position];
        if (line.contains(if_directive)) {
            ++nesting;
        } else if (line.contains(endif_directive)) {
            --nesting;
        }
        if (nesting == 0) {
            break;
        }
        ++parser_position;
    }
}

Interpreter_ns::Global::Global() {
    global_parameters = new std::map<std::string, std::string>();
    global_functions  = new std::map<std::string, glob_func>();
}

Interpreter_ns::Global::~Global() {
    delete global_parameters;
    delete global_functions;
}

bool Interpreter_ns::Global::empty() const {
    return global_parameters->empty() && global_functions->empty();
}

bool Interpreter_ns::Global::contains_global(const std::string &key) const {
    return global_parameters->contains(key);
}

void Interpreter_ns::Global::add_global_parameters(const std::string &key, const std::string &value) const {
    global_parameters->insert_or_assign(key, value);
}

void Interpreter_ns::Global::add_global_functions(const std::string &key, glob_func value) const {
    global_functions->insert_or_assign(key, value);
}

void Interpreter_ns::Global::remove_global_parameters(const std::string &key) const {
    global_parameters->erase(key);
}

Interpreter_ns::glob_func Interpreter_ns::Global::get_global_function(const std::string &key) const {
    if (global_functions->contains(key)) {
        return global_functions->at(key);
    }
    throw Check_exceptions::LineParserException(__LINE__,
                                                ("Global function not found: " + key).c_str(), __FILE_NAME__);
}

std::string Interpreter_ns::Global::get_global_parameter(const std::string &key) const {
    if (global_parameters->contains(key)) {
        return global_parameters->at(key);
    }
    throw Check_exceptions::LineParserException(__LINE__,
                                                ("Global function not found: " + key).c_str(), __FILE_NAME__);
}

/**
 * Initialize global functions in interpreter
 */
void Interpreter_ns::Global::init_global_functions() const {
    global_functions->insert({
        "Contains", [](const std::vector<std::string> &args) {
            if (args.size() != 2) {
                return false;
            }
            return args[0].starts_with(args[1]);
        }
    });

    global_functions->insert({
        "Starts_with", [](const std::vector<std::string> &args) {
            if (args.size() != 2) {
                return false;
            }
            return args[0].starts_with(args[1]);
        }
    });

    global_functions->insert({
        "Empty", [](const std::vector<std::string> &args) {
            if (args.size() != 1) {
                return false;
            }
            return args[0].starts_with(args[1]);
        }
    });

#ifdef EXPERIMENTAL
#pragma message("Using experimental built in functions")

    global_functions->insert({
        "Error", [](const std::string &msg) {
            printf(msg.c_str());
        }
    });

    global_functions->insert({
        "Message", [](const std::string &msg) {
            printf(msg.c_str());
        }
    });
#endif
}

bool Interpreter_ns::Global::call_global_function(const std::string &name, const std::string &args) const {
    if (!global_functions->contains(name)) {
        throw Check_exceptions::LineParserException(__LINE__,
                                                    ("Unknown global function: " + name).c_str(), __FILE_NAME__);
    }

    std::vector<std::string> parsed_args;
    auto                     raw_args = libio::string::split(args, ",");

    for (auto &arg: raw_args) {
        arg = libio::string::delete_whitespaces(arg);

        if (arg.starts_with("\"") && arg.ends_with("\"")) {
            arg = arg.substr(1, arg.length() - 2);
        }

        if (global_parameters && global_parameters->contains(arg)) {
            arg = global_parameters->at(arg);
        }

        parsed_args.push_back(arg);
    }
    return global_functions->at(name)(parsed_args);
}
