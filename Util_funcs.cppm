/*
 Header file with some useful functions
 */

module;

#include <filesystem>
#include <regex>
#include "termcolor.hpp"
#include <source_location>

/**
 Module with different useful functions.
 */
export module UtilFuncs_mod;

/**
 Namespace with utility functions of the C+ compiler
 */
namespace utility {
    /**
     Enumerate class for compiler output colors
     */
    export enum Color {
        WHITE,
        RED,
        BLUE,
        GREEN
    };

    /**
     Function for printt value into standard output with next line.
     @param value element to print
     */
    export template<typename T = std::string>
    void println(const T &value = "\n") {
        std::cout << value << "\n";
    }

    /**
     Function for print value into standard output without next line.
     @param value element to print
     @param separator string for separate output
     */
    export template<typename T>
    void print(const T &value, const std::string &separator = " ") {
        std::cout << value << separator;
    }

    /**
     Data object with useful ansi things
     */
    export struct [[maybe_unused]] Ansi_utils {
    private:
        std::string red_start = "\033[91m";

        std::string color_end = "\033[00m";

        std::string green_start = "\033[92m";

        std::string yellow_start = "\033[93m";

        std::string cyan_start = "\033[96m";

        std::string gray_start = "\033[97m";

    public:
        /**
         * Print given string in red color
         * @param value const string representing value to print
         */
        void prRed(const std::string &value) const {
            print(red_start + value + color_end);
        }

        /**
         * Print given string in green color
         * @param value const string representing value to print
         */
        void prGreen(const std::string &value) const {
            print(green_start + value + color_end);
        }

        /**
        * Print given string in yellow color
        * @param value const string representing value to print
        */
        void prYellow(const std::string &value) const {
            print(yellow_start + value + color_end);
        }

        /**
        * Print given string in cyan color
        * @param value const string representing value to print
        */
        void prCyan(const std::string &value) const {
            print(cyan_start + value + color_end);
        }

        /**
        * Print given string in light gray color
        * @param value const string representing value to print
        */
        void prLightGray(const std::string &value) const {
            print(gray_start + value + color_end);
        }
    };

    /**
     * Function for searching string in source string
     * @param source in which string find another string
     * @param string_if_contains string that might be in source string
     * @return bool value if string is in source
     */
    export [[maybe_unused]] inline bool contains(const std::string &source, const std::string &string_if_contains) {
        return source.find(string_if_contains);
    }

    /**
     Function for text output into standard output with color.
     Color provided by termcolor library.
     @param str string for output in console
     @param color color value from Color enum
     */
    export [[maybe_unused]] inline void colored_txt_output(const std::string &str, const Color &color = Color::WHITE) {
        switch (color) {
            case Color::WHITE:
                std::cout << termcolor::white;
                break;
            case Color::RED:
                std::cout << termcolor::red;
                break;
            case Color::BLUE:
                std::cout << termcolor::blue;
                break;
            case Color::GREEN:
                std::cout << termcolor::green;
                break;
            default:
                std::cerr << "Color do not specified";
                throw std::exception();
        }
        std::cout << str;
        std::cout << termcolor::nocolorize;
    }

    /**
    Function for printing message in red
    @param error error to print
     */
    export [[maybe_unused]] inline void print_error(const std::string &error) {
        std::cout << termcolor::red << error;
    }

    /**
    Function for printing message in green
    @param success msg to print
    */
    export [[maybe_unused]] inline void print_success(const std::string &success) {
        std::cout << termcolor::green << success;
    }

    /**
    Function for transferring ASCII to bool value.
    Yes, I know that this function is something that smell, but...
    @param string_to_scan string object to "cast" into bool value.
    @return bool value for translate
    */
    export [[maybe_unused]] inline bool atob(const std::string &string_to_scan) {
        if (string_to_scan == "true") {
            return true;
        }
        if (string_to_scan == "false") {
            return false;
        }
        if (string_to_scan == "True") {
            return true;
        }
        if (string_to_scan == "False") {
            return false;
        }
        colored_txt_output("Error occurred in atob function", Color::RED);
        return false;
    }

    /**
    Function for replacing symbol c1 with c2 (char version).
    @param s source string.
    @param c1 which char to replace.
    @param c2 replace source char.
    @return string value with replacing symbols.
    */
    export [[maybe_unused]] std::string replace(std::string &s, const char c1, const char c2) {
        const auto l = s.length();
        for (int i = 0; i < l; i++) {
            if (s[i] == c1) {
                s[i] = c2;
            } else if (s[i] == c2) {
                s[i] = c1;
            }
        }
        return s;
    }

    /**
    Function for replacing string c1 with c2 (string version).
    @param s source string.
    @param s1 string to replace in source
    @param s2 replace source string.
    @return string value with replacing symbols.
    */
    export [[maybe_unused]] std::string replace(const std::string &s, const std::string &s1, const std::string &s2) {
        const auto l = s.size();
        for (int i = 0; i < l; i++) {
            //
        }
        return "";
    }

    /**
    Function for splitting string into array of strings.
    @param to_split source string
    @param delim separator for splitting
    @return vector object with strings
    */
    export [[maybe_unused]] std::vector<std::string> line_splitter(const std::string &to_split, const char delim = ' ') {
        std::istringstream input{to_split};
        std::vector<std::string> result;
        std::string tmp;
        // extract substrings one-by-one
        while (getline(input, tmp, delim)) {
            input >> tmp;
            result.push_back(tmp);
        }
        return result;
    }

    /**
    trim some characters at sequence end
    @param seq
    @param pred
    @return
     */
    export template<typename Sequence, typename Pred>
    [[maybe_unused]] Sequence &trim_end(Sequence &seq, Pred pred) {
        auto last = std::find_if_not(seq.rbegin(), seq.rend(), pred);
        seq.erase(last.base(), seq.end());
        return seq;
    }

    /**
    trim some characters at sequence start
    @param seq
    @param pred
    @return
     */
    export template<typename Sequence, typename Pred>
    [[maybe_unused]] Sequence &trim_start(Sequence &seq, Pred pred) {
        auto first = std::find_if_not(seq.begin(), seq.end(), pred);
        seq.erase(seq.begin(), first);
        return seq;
    }

    /**
     Function for replacing all strings in string
     @param str source string
     @param replace replace this in source
     @param with replace with this string
     @return
     */
    export [[maybe_unused]] inline std::string &replace_string_all(std::string &str, const std::string &replace, const std::string &with) {
        if (!replace.empty()) {
            std::size_t pos = 0;
            while ((pos = str.find(replace, pos)) != std::string::npos) {
                str.replace(pos, replace.length(), with);
                pos += with.length();
            }
        }
        return str;
    }

    /**
     Function for replace string with another string
     @param str source string
     @param replace replace what
     @param with replace with string
     @return string with replacement
     */
    export [[maybe_unused]] inline std::string &replace_string(std::string &str, const std::string &replace, const std::string &with) {
        std::size_t pos = str.find(replace);
        if (pos != std::string::npos)
            str.replace(pos, replace.length(), with);
        return str;
    }

    /**
    Function for converting from char array into string array.
    @param input_array char array.
    @param size - size of the char array.
    @return converted string
    */
    export [[maybe_unused]] inline std::string convert_to_string(const std::string &input_array, const int size) {
        std::string s;
        for (int i = 0; i < size; i++) {
            s += input_array[i];
        }
        return s;
    }

    /**
     * Trim string, to remove trailing whitespaces
     * @param str
     * @param whitespace
     * @return
     */
    export std::string trim(const std::string &str, const std::string &whitespace = " \t") {
        const auto strBegin = str.find_first_not_of(whitespace);
        if (strBegin == std::string::npos) {
            return ""; // no content
        }
        const auto strEnd = str.find_last_not_of(whitespace);
        const auto strRange = strEnd - strBegin + 1;
        return str.substr(strBegin, strRange);
    }

    /**
    * Writes down value into variable by address.
    * @tparam T generic type.
    * @param variableAddress address of variable to output data to it.
    */
    export template<typename T>
    void userInput(T &variableAddress) {
        if (std::cin.good()) {
            std::cin >> variableAddress;
        } else {
            throw std::runtime_error("Input stream is not good");
        }
    }

    /**
     * Different variant for userInput.
     * @tparam T generic type for variable.
     * @return variable of generic type.
     */
    export template<typename T = std::string>
    T userInput() {
        T variableAddress;
        if (std::cin.good()) {
            std::cin >> variableAddress;
        }
        return variableAddress;
    }

    /**
     * Access line and column of the log.
     * @param message message to print to console.
     * @param location source location object to search for line and column of execution.
     */
    export void message_with_location(const std::string &message, const std::source_location location = std::source_location::current()) {
        std::cout << location.file_name() << ":" << location.line() << ":" << location.column() << " (" << location.function_name() << "): " <<
                message << "." << "\n";
    }

    /**
    * Platform independent filepath getter.
    * @deprecated because crashes program.
    * @return string value of current path
    */
    export inline std::string getCwd() {
        const std::filesystem::path currentPath = std::filesystem::current_path();
        return currentPath.string();
    }

    /**
     * Split string into vector or create another vector and return it.
     * @param s source string to split
     * @param delim delimiter to split on
     * @param elems vector with strings to add split string
     * @return vector if you want to assign to variable.
     */
    export std::vector<std::string> &split(const std::string &s, std::vector<std::string> &elems, const char delim = ' ') {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
}
