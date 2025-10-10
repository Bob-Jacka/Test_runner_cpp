#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP

#include <string>
#include <vector>

#include "File_controller.hpp"
#include "../Data/Constants.hpp"

/**
 * Namespace for interpreter
 */
namespace Line_interpreter_ns {
    class DirectiveInterpreter {
        mutable int interpreter_position = 0; ///position of interpreter in main suit of utility

    public:
        void directive_group(std::vector<std::string> &, const std::string &) const;

        void directive_if(const std::string &) const;

        void directive_elif(const std::string &) const;

        void directive_else() const;

        std::vector<std::string> directive_import(const std::string &) const;

        //interpreter other actions
        [[nodiscard]] int get_interpreter_position() const;

        void set_interpreter_position(const int &);
    };
}

/**
 * Line interpreter of utility
 */
class Line_parser {
    std::vector<std::string> main_suit; ///main vector with lines
    Line_interpreter_ns::DirectiveInterpreter interpreter;

    std::vector<std::string> import_suits(File_controller *file_controller) const;

public:
    explicit Line_parser();

    explicit Line_parser(const std::vector<std::string> &) = delete;

    Line_parser(const Line_parser &) = delete;

    Line_parser &operator=(const Line_parser &) = delete;

    ~Line_parser() = default;

    [[nodiscard]] std::vector<std::string> get_main_suit() const;

    void set_main_suits(const std::vector<std::string> &main_suits);

    void parse_lines_empty() const; ///delete comments elements from vector

    void parse_directives() const; ///method for parsing directives in test suit file.
};

#endif
