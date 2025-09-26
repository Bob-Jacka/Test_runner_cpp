#ifndef LINEPARSER_HPP
#define LINEPARSER_HPP

#include <string>
#include <vector>

#include "../Data/Constants.hpp"

class Line_parser {
    std::vector<std::string> import_outer_suits() const;

public:
    explicit Line_parser();

    explicit Line_parser(const std::vector<std::string> &) = delete;

    Line_parser(const Line_parser &) = delete;

    Line_parser &operator=(const Line_parser &) = delete;

    ~Line_parser() = default;

    [[nodiscard]] std::vector<std::string> &parse_lines_empty(std::vector<std::string> &) const; ///delete comments elements from vector

    [[nodiscard]] std::vector<std::string> parse_directives(std::vector<std::string> &) const; ///method for parsing directives in test suit file.
};

#endif
