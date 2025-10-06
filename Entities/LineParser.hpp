#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP

#include <string>
#include <vector>

#include "File_controller.hpp"
#include "../Data/Constants.hpp"

/**
 * Line interpreter of utility
 */
class Line_parser {
    std::vector<std::string> main_suit; ///main vector with lines
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
