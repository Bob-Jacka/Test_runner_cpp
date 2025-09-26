#include "LineParser.hpp"

std::vector<std::string> Line_parser::import_outer_suits() const {
    //
}

Line_parser::Line_parser() {
    //
}

std::vector<std::string> Line_parser::parse_directives(std::vector<std::string> &lines) const {
    //
}

std::vector<std::string> &Line_parser::parse_lines_empty(std::vector<std::string> &lines) const {
    for (const auto &line: lines) {
        if (line.starts_with(comment)) {
            lines.erase();
        }
    }
}
