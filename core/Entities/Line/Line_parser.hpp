#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP

#include <map>
#include "../File_controller.hpp"
#include "../../Data/Constants.hpp"
#include "../../Exceptions/LineParserException.hpp"

import Libio;
import UtilFuncs_mod;

/**
 * Namespace for directive parser and other things
 */
namespace Interpreter_ns {
    typedef bool (*glob_func)(const std::vector<std::string> &); //using c-style functions instead of functional

    /**
     * Global statement of the parser, i.e. Global scope in parser
     */
    struct Global {
        private:
            std::map<std::string, std::string> *global_parameters; ///global parameters, key is param name, value is param value
            std::map<std::string, glob_func> *  global_functions;

            void init_global_functions() const;

            [[nodiscard]] bool call_global_function(const std::string &name, const std::string &args) const;

        public:
            Global();

            ~Global();

            [[nodiscard]] bool empty() const;

            [[nodiscard]] bool contains_global(const std::string &key) const;

            void remove_global_parameters(const std::string &key) const;

            void add_global_parameters(const std::string &key, const std::string &value) const;

            void add_global_functions(const std::string &key, glob_func value) const;

            [[nodiscard]] glob_func get_global_function(const std::string &key) const;

            [[nodiscard]] std::string get_global_parameter(const std::string &key) const;
    };

    /**
     * Class for directive parser
     */
    class Directive_parser final {
        private:
            int parser_position; ///position of parser in main suit of utility

            std::vector<std::string> inner_vector_to_proceed; ///main vector with lines

            std::vector<std::string> *output_vector; ///vector that will be output result of parse directives and further creating test cases

            Global *global_map; ///global map for storing parameters and functions

        private:
            [[nodiscard]] bool interpret_logical_expression(const std::string &) const;

            void parse_global_func();

            bool jmp_to(const std::string &);

            bool high_level_branch_wrapper(const std::string &_arguments);

            void include_cycle();

            void skip_to_next_branch();

            void skip_to_endif();

        public:
            void directive_group(const std::string &);

            void parse_parameters(const std::string &) const;

            bool process_block_until_next();

            //constructor and destructor:
            Directive_parser();

            ~Directive_parser() = default;

            explicit Directive_parser(const std::vector<std::string> &) = delete;

            Directive_parser(const Directive_parser &) = delete;

            Directive_parser(const Directive_parser &&) = delete;

            Directive_parser &operator=(const Directive_parser &) = delete;

            [[nodiscard]] std::vector<std::string> get_output_vector() const;

            //Other methods:

            std::vector<std::string> exec(const std::vector<std::string> &); ///main method in directive parser

            std::vector<std::string> delete_comments(std::vector<std::string> &) const; ///delete comments elements from vector

            void add_to_output_vector(const std::vector<std::string> &) const;

            //parser other actions
            void set_parser_position(const int &);
    };
}

#endif
