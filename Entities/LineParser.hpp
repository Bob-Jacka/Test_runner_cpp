#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP

#include <string>
#include <vector>

#include "../Data/Constants.hpp"

class Line_parser {
    void import_outer_suits();

public:
    explicit Line_parser();

    explicit Line_parser(const std::vector<std::string> &) = delete;

    Line_parser(const Line_parser &) = delete;

    Line_parser &operator=(const Line_parser &) = delete;

    std::vector<std::string> parse_lines_empty(std::vector<std::string>) const; ///delete comments elements from vector

    void parse_directives() const; ///method for parsing directives in test suit file.
};

#endif
