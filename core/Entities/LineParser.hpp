#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP

#include <map>
#include <string>
#include <vector>

#include "File_controller.hpp"
#include "../Data/Constants.hpp"
#include "../Exceptions/LineInterpreterException.hpp"

/**
 * Namespace for directive interpreter and other things
 */
namespace Interpreter_ns {
    /**
     * Class for directive interpreter
     */
    class DirectiveInterpreter {
        mutable int interpreter_position; ///position of interpreter in main suit of utility

        mutable std::map<std::string, std::string> suit_parameters; ///variables of suit

        mutable std::vector<std::string> inner_vector_to_proceed; ///main vector with lines

        mutable std::vector<std::string> output_vector; ///vector that will be output result of parse directives

        bool interpret_logical_expression(const std::string &) const;

        bool jmp_to(const std::string &) const;

        bool high_level_branch_wrapper(const std::string &_arguments) const;

    public:
        void directive_group(const std::string &) const;

        void parse_parameters(const std::string &) const;

        //constructor and destructor:
        DirectiveInterpreter();

        ~DirectiveInterpreter() = default;

        explicit DirectiveInterpreter(const std::vector<std::string> &) = delete;

        DirectiveInterpreter(const DirectiveInterpreter &) = delete;

        DirectiveInterpreter &operator=(const DirectiveInterpreter &) = delete;

        //parameters getter:
        std::map<std::string, std::string> get_suit_parameters() const;

        std::vector<std::string> get_output_vector() const;

        //Other methods:

        std::vector<std::string> parse_directives(const std::vector<std::string> &) const;

        std::vector<std::string> preprocess_lines(std::vector<std::string> &) const; ///delete comments elements from vector

        void add_to_output_vector(const std::vector<std::string> &) const;

        //interpreter other actions
        [[nodiscard]] int get_interpreter_position() const;

        void set_interpreter_position(const int &) const;

        void increment_interpreter_position() const;
    };
}

#endif
