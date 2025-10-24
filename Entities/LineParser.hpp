#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP

#include <map>
#include <string>
#include <vector>

#include "File_controller.hpp"
#include "../Data/Constants.hpp"

/**
 * Namespace for directive interpreter and other things
 */
namespace Line_interpreter_ns {
    /**
     * Class for directive interpreter
     */
    class DirectiveInterpreter {
        mutable int interpreter_position; ///position of interpreter in main suit of utility

        mutable std::map<std::string, std::string> suit_parameters; ///variables of suit

        std::vector<std::string> main_suit; ///main vector with lines

        std::vector<std::string> import_suits(File_controller *) const;

        bool interpret_logical_expression(const std::string &) const;

    public:
        void directive_group(std::vector<std::string> &, const std::string &) const;

        void directive_if(const std::string &) const;

        void directive_elif(const std::string &) const;

        void directive_else() const;

        std::vector<std::string> directive_import(const std::string &) const;

        //constructor and destructor:
        DirectiveInterpreter();

        explicit DirectiveInterpreter(const std::vector<std::string> &) = delete;

        DirectiveInterpreter(const DirectiveInterpreter &) = delete;

        DirectiveInterpreter &operator=(const DirectiveInterpreter &) = delete;

        ~DirectiveInterpreter() = default;

        //parameters getter:
        std::map<std::string, std::string> get_suit_parameters() const;

        //Other methods:
        void parse_parameters(const std::string &) const;

        [[nodiscard]] std::vector<std::string> get_main_suit() const;

        void set_main_suits(const std::vector<std::string> &);

        void parse_directives() const;

        void parse_lines_empty() const; ///delete comments elements from vector

        //interpreter other actions
        [[nodiscard]] int get_interpreter_position() const;

        void set_interpreter_position(const int &) const;

        void increment_interpreter_position() const;

        void add_till_line_starts(const std::string &) const;
    };
}

#endif
