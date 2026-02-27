#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP

#ifdef GLOBAL_MAPS
#include <map>
#endif

#include "../File_controller.hpp"
#include "../../Data/Constants.hpp"
#include "../../Exceptions/LineInterpreterException.hpp"

import Libio;
import UtilFuncs_mod;

/**
 * Namespace for directive interpreter and other things
 */
namespace Interpreter_ns {
#ifdef GLOBAL_MAPS
    /**
     * Global statement of the parser
     */
    template<typename K = std::string, typename V = std::string>
    struct Global {
        private:
            std::map<K, V> *global_parameters; ///global parameters, key is param name, value is param value
            std::map<K, V> *global_functions;

        public:
            Global() = default;

            ~Global() = default;

            bool contains_global(K key);

            void add_global_parameters(const K &key, const V &value);

            void remove_global_parameters(const K &key);

            void add_global_functions(const K &key, const V &value);

            V get_global_function(K key);
    };
#endif

    /**
     * Class for directive interpreter
     */
    class Directive_interpreter final {
        private:
            int interpreter_position; ///position of interpreter in main suit of utility

            std::vector<std::string> inner_vector_to_proceed; ///main vector with lines

            std::vector<std::string> *output_vector; ///vector that will be output result of parse directives and further creating test cases

        private:
            [[nodiscard]] static bool interpret_logical_expression(const std::string &);

            void parse_global_func();

            bool jmp_to(const std::string &);

            bool high_level_branch_wrapper(const std::string &_arguments);

            void include_cycle();

        public:
            void directive_group(const std::string &);

            void parse_parameters(const std::string &) const;

            //constructor and destructor:
            Directive_interpreter();

            ~Directive_interpreter() = default;

            explicit Directive_interpreter(const std::vector<std::string> &) = delete;

            Directive_interpreter(const Directive_interpreter &) = delete;

            Directive_interpreter(const Directive_interpreter &&) = delete;

            Directive_interpreter &operator=(const Directive_interpreter &) = delete;

#ifdef GLOBAL_MAPS
            //parameters getter:
            [[nodiscard]] std::map<std::string, std::string> get_suit_parameters() const;
#endif

            [[nodiscard]] std::vector<std::string> get_output_vector() const;

            //Other methods:

            std::vector<std::string> exec(const std::vector<std::string> &); ///main method in directive interpreter

            std::vector<std::string> delete_comments(std::vector<std::string> &) const; ///delete comments elements from vector

            void add_to_output_vector(const std::vector<std::string> &) const;

            //interpreter other actions
            void set_interpreter_position(const int &);
    };
}

#endif
