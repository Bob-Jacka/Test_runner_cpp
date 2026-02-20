#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP

#include <map>
#include <string>
#include <vector>

#include "../File_controller.hpp"
#include "../../Data/Constants.hpp"
#include "../../Exceptions/LineInterpreterException.hpp"

/**
 * Namespace for directive interpreter and other things
 */
namespace Interpreter_ns {
    using Parameters = std::map<std::string, std::string>; ///Map object for parameters

    /**
     * Class for directive interpreter
     */
    class Directive_interpreter {
    private:
        int interpreter_position; ///position of interpreter in main suit of utility

        Parameters global_parameters; ///global parameters, key is param name, value is param value

        std::vector<std::string> inner_vector_to_proceed; ///main vector with lines

        std::vector<std::string> output_vector; ///vector that will be output result of parse directives and further creating test cases

    private:
        [[nodiscard]] static bool interpret_logical_expression(const std::string &);

        bool jmp_to(const std::string &);

        bool high_level_branch_wrapper(const std::string &_arguments);

        void include_cycle();

    public:
        void directive_group(const std::string &);

        void parse_parameters(const std::string &);

        //constructor and destructor:
        Directive_interpreter();

        ~Directive_interpreter() = default;

        explicit Directive_interpreter(const std::vector<std::string> &) = delete;

        Directive_interpreter(const Directive_interpreter &) = delete;

        Directive_interpreter &operator=(const Directive_interpreter &) = delete;

        //parameters getter:
        Parameters get_suit_parameters();

        [[nodiscard]] std::vector<std::string> get_output_vector() const;

        //Other methods:

        std::vector<std::string> exec(const std::vector<std::string> &); ///main method in directive interpreter

        std::vector<std::string> delete_comments(std::vector<std::string> &); ///delete comments elements from vector

        void add_to_output_vector(const std::vector<std::string> &);

        //interpreter other actions
        void set_interpreter_position(const int &);
    };
}

#endif
