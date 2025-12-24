#ifndef INIPARSER_HPP
#define INIPARSER_HPP

#include <vector>
#include <string>

#include <map>
#include <exception>
#include <source_location>

import Libio;

namespace Interpreter_ns {
    class ParserException final : public std::exception {
        std::string message;

    public:
        explicit ParserException(const std::string &, const std::source_location & = std::source_location::current());

        [[nodiscard]] const char *what() const noexcept override;

        ~ParserException() override = default;
    };

    constexpr auto start_section = '[';
    constexpr auto end_section = ']';
    constexpr auto value_splitter = '=';
    constexpr auto comment_start = ';';

    /**
     * Main class of the ini parser.
     * Parse ini file.
     */
    class Ini_parser {
        using sections_type = std::map<std::string, std::map<std::string, std::string> >;

        sections_type sections;

        sections_type parse_ini_from_vector(const std::vector<std::string> &lines);

    public:
        explicit Ini_parser(const std::string &);

        Ini_parser() = delete;

        ~Ini_parser() = default;

        Ini_parser(const Ini_parser &) = delete;

        template<typename T>
            requires std::is_integral_v<T> || libio::type_constrains::is_string<T>
        T get_value(const std::string &) const;
    };
}

/**
 * Get value from section object
 * @tparam T generic param
 * @param section_param string value
 * @return generic parameter value from section
 */
template<typename T>
    requires std::is_integral_v<T> || libio::type_constrains::is_string<T>
T Interpreter_ns::Ini_parser::get_value(const std::string &section_param) const {
    if (section_param.contains(".")) {
        const auto split_line = libio::string::split(section_param,
                                                     "."); //0 - name of the section and 1 - name of the value
        for (const auto &section: sections) {
            //key is section name, value is section
            if (split_line[0] == sect) {
                try {
                    if (section.second.contains(split_line[1])) {
                        auto to_return = libio::string::convert_to_t<T>(section.second.at(split_line[1]));
                        return to_return;
                    }
                } catch (const std::exception &e) {
                    throw ParserException("Error retrieving value: " + std::string(e.what()));
                }
            }
        }
        for (const auto &kv: sections.at(split_line[0])) {
            libio::output::println("  " + kv.first + "=" + kv.second);
        }
        libio::output::println();

        throw ParserException("No value, but maybe you mistyped, upper you will see some values from section");
    }
    throw ParserException("Unknown section name");
}

#endif
