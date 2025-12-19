module;

/**
 Custom library for actions in Netology C++ course and later for more serious projects.
 Version - 1.20.0
 This library could be a module, but yes, later rewritten to module with LIBIO_EXPERIMENTAL functions.
 Some kind of Boost library for poor people.
*/

#include <fstream>
#include <iostream>
#include <regex>
#include <cstring>

// #define LIBIO_EXPERIMENTAL //uncomment/comment this line to turn on/off LIBIO_EXPERIMENTAL library features
// #define LIBIO_TEST //uncomment/comment this line to turn on/off library test
// #define UNSTABLE //turns on unstable versions of very popular functions

#ifdef LIBIO_EXPERIMENTAL ///define functions and include other libraries if LIBIO_EXPERIMENTAL tag is defined
#include <filesystem>
#include <cmath>
#incldue <vector>
#include <iterator>
#include <sstream>
#endif

export module Libio;

/**
 * Unified namespace for libio library for input/output.
 */
namespace libio {
    using cint [[maybe_unused]] = const int; ///constant custom integer type
    using cbool [[maybe_unused]] = const bool; ///constant custom bool type

    /**
     * Namespace for constrains of types using concepts
     */
    export namespace type_constrains {
        template<typename T>
        concept is_string = std::is_same_v<T, std::string>;

        /**
         * Check for standard library container
         */
        template<typename T>
        concept is_stl_container = requires(T t)
        {
            // Type must have an iterator type
            typename T::iterator;

            // Must have a begin() method returning an iterator
            { t.begin() } -> std::same_as<typename T::iterator>;

            // Must have an end() method returning an iterator
            { t.end() } -> std::same_as<typename T::iterator>;
        };
    }

    /**
     * Contains different output logic
     */
    namespace output {
        /**
         * Namespace for colored text output
         */
        namespace colored {
            /**
            Enumerate class for compiler output colors
            */
            export enum Color {
                WHITE,
                RED,
                BLUE,
                GREEN
            };

            struct Ansi_colors {
                //
            };

            export template<typename T>
            void colored_print(const T &str, const std::string &separator = " ", Color color = WHITE) {
                //
            }
        }


#ifdef UNSTABLE
        /**
         * Print given generic message in console with new line. By default, equal to "".
         * @warning If using C++23 - use std::println.
         * @param str string to output
         * @tparam T generic parameter of type to console print
         */
        export template<typename T = std::string>
        void println(const T &str = "\n") {
            std::cout << str << std::endl;
        }

        /**
         * Print given generic message in console without new line.
         * @warning If using C++23 - use std::print.
         * @tparam T generic type
         * @param str string to output
         * @param separator text separator
         */
        export template<typename T>
        void print(const T &str, std::string separator = "") {
            std::cout << str << separator;
        }

#elifndef UNSTABLE

        /**
         * Print given generic message in console with new line. By default, equal to "".
         * @warning If using C++23 - use std::println.
         * @param str string to output
         * @tparam T generic parameter of type to console print
         */
        export template<typename T = std::string>
        void println(const T &str = "\n") {
            if (std::cout.good()) {
                std::cout << str << std::endl;
            }
        }

        /**
         * Print given generic message in console without new line.
         * @warning If using C++23 - use std::print.
         * @tparam T generic type
         * @param str string to output
         * @param separator text separator
         */
        export template<typename T>
        void print(const T &str, std::string separator = "") {
            if (std::cout.good()) {
                std::cout << str << separator;
            }
        }

#endif

        /**
         * Function for array output with separator.
         * Older brother of dynamicArrayOutput
         * @param array generic array pointer
         * @param array_size size of array.
         * @param separator separator value between elements
         * @param is_inline I do not know why I do this.
         */
        export template<typename T>
        void lineArrayOutput(const T *array, const int array_size, const std::string &separator = " ",
                             const bool is_inline = false) {
            for (int i = 0; i < array_size - 1; ++i) {
                std::cout << array[i] << separator;
            }
            std::cout << array[array_size - 1];

            if (!is_inline) {
                std::cout << std::endl;
            }
        }

        /**
         * Output container from STL into console
         * @tparam T generic type
         * @param array standard container
         * @param separator separator string
         * @param endsymbol symbol at the end of output sequence
         */
        export template<typename T>
            requires std::copyable<T>
        void lineArrayOutput(T array, const std::string &separator = " ", const std::string &endsymbol = "") {
            const size_t array_size = array.size();
            int i = 0;
            for (; i < array_size - 1; ++i) {
                std::cout << array[i] << separator;
            }
            std::cout << array[i] << endsymbol;
        }

        /**
         * New technology parametrized function for array output with old innovations
         * @tparam T generic type
         * @param array generic array pointer.
         * @param size size of the array.
         * @param reverse order of array output, seq or reverse.
         * @param separator separator value between elements
         */
        export template<typename T>
        void dynamicArrayOutput(const T *array, const int size, const bool reverse = false, const std::string &separator = " ") {
            if (reverse) {
                for (int i = size - 1; i >= 0; --i) {
                    std::cout << array[i] << separator;
                }
            } else {
                for (int i = 0; i < size; ++i) {
                    std::cout << array[i] << separator;
                }
            }
            std::cout << std::endl;
        }

        /**
         * Weird construction for correct output separator.
         * @tparam T generic type
         * @param container container object to print out
         * @param separator separator value between values
         */
        export template<typename T>
            requires std::copyable<T>
        void print_container(const T &container, const std::string &separator = " ") {
            const size_t container_size = container.size();
            int i = 0;
            for (const auto &elem: container) {
                if (i < container_size - 1) {
                    std::cout << elem << separator;
                } else {
                    std::cout << elem;
                }
                ++i;
            }
            std::cout << std::endl;
        }

#ifdef LIBIO_EXPERIMENTAL
        /**
        * Print pyramid object one line by line
        * @param array
        * @param n
        */
        export void print_pyramid(const int *array, const int n) {
            for (int i = 0; i < n; ++i) {
                const int level = (i == 0) ? 0 : static_cast<int>(std::floor(std::log2(static_cast<double>(i + 1))));

                if (i == 0) {
                    std::cout << level << " root " << array[i] << '\n';
                } else {
                    const int p = (i - 1) / 2;
                    const char *side = (i == 2 * p + 1) ? "left" : "right";
                    std::cout << level << " " << side << " (" << array[p] << ") " << array[i] << '\n';
                }
            }
        }

        /**
         *
         * @param array pyramid object in array
         * @param i
         */
        export void print_one_element(const int *array, const int i) {
            const int level = (i == 0) ? 0 : static_cast<int>(std::floor(std::log2(static_cast<double>(i + 1))));

            if (i == 0) {
                std::cout << level << " root " << array[i] << '\n';
            } else {
                const int p = (i - 1) / 2;
                const char *side = (i == 2 * p + 1) ? "left" : "right";
                std::cout << level << " " << side << " (" << array[p] << ") " << array[i] << '\n';
            }
        }

#endif
    }

    /**
     * Namespace for string actions in libio
     */
    export namespace string {
        /**
         * Split string without separator
         * @param input input string to split
         * @return vector object with strings
         */
        std::vector<std::string> split(std::string const &input) {
            std::stringstream ss(input);
            std::vector<std::string> result;
            std::string word;
            while (ss >> word) {
                result.push_back(word);
            }
            return result;
        }

        /**
        * Split string into vector object and return changed string.
        * @param s source string to split.
        * @param delim delimiter to split on.
        * @return vector if you want to assign to variable.
        */
        std::vector<std::string> split(const std::string &s, const std::string &delim = " ") {
            std::vector<std::string> result;
            const std::regex del(delim);
            std::sregex_token_iterator it(s.begin(),
                                          s.end(), del, -1);
            const std::sregex_token_iterator end;
            while (it != end) {
                result.push_back(*it);
                ++it;
            }
            return result;
        }

        /**
         * Delete whitespaces at begin and end of the given string.
         * @param s source string.
         * @return string object without whitespaces.
         */
        std::string delete_whitespaces(const std::string &s) {
            const size_t first_char_pos = s.find_first_not_of(" \t\n\r\f\v");
            std::string output_string = s;
            if (first_char_pos != std::string::npos) {
                output_string.erase(0, first_char_pos);
            }
            return output_string;
        }

        /**
         * Change string register by invoking std functions
         * @param str source string value.
         * @param regis output string register, can be either false (upper) or true (lower).
         * @param loc localization
         * @return string in selected register.
         */
        std::string change_string_register(const std::string &str, const bool regis, const std::locale &loc = std::locale("en")) {
            decltype(auto) func = (!regis) ? std::toupper<char> : std::tolower<char>;
            std::string result;
            for (const auto ch: str) {
                result += func(ch, loc);
            }
            return result;
        }

        /**
         Function for replacing all strings in string
         @param str source string
         @param replace replace this in source
         @param with replace with this string
         @return string with replacements
         */
        [[maybe_unused]] inline std::string &replace_string_all(std::string &str, const std::string &replace, const std::string &with) {
            if (!replace.empty()) {
                std::size_t pos = 0;
                while ((pos = str.find(replace, pos)) != std::string::npos) {
                    str.replace(pos, replace.length(), with);
                    pos += with.length();
                }
            }
            return str;
        }
    }

    /**
     * Contains different input logic.
     */
    export namespace input {
        /**
         * Writes down int value into variable by address
         * @param variableAddress address of variable to output data to it.
         */
        inline void intUserInput(int &variableAddress) {
            if (std::cin.good()) {
                std::cin >> variableAddress;
            }
        }

        /**
         * Writes down long value into variable by address
         * @param variableAddress address of variable to output data to it.
         */
        inline void longUserInput(long &variableAddress) {
            if (std::cin.good()) {
                std::cin >> variableAddress;
            }
        }

        /**
         * Writes down string into variable by address
         * @param variableAddress address of variable to output data to it.
         */
        inline void stringUserInput(std::string &variableAddress) {
            if (std::cin.good()) {
                std::cin >> variableAddress;
            }
        }

        /**
         * Writes down value into variable by address.
         * @tparam T generic type.
         * @param variableAddress address of variable to output data to it.
         */
        template<typename T>
        void userInput(T &variableAddress) {
            if (std::cin.good()) {
                std::cin >> variableAddress;
            }
        }

        /**
        * Different variant for userInput.
        * @tparam T generic type for variable.
        * @return variable of generic type.
        */
        template<typename T = std::string>
        T userInput() {
            T variable;
            if (std::cin.good()) {
                std::cin >> variable;
            }
            return variable;
        }

        /**
         * @param input_symbol symbol that appear in start of inputting
         * @return user string
         */
        std::string lineInput(const std::string &input_symbol) {
            std::string line;
            std::cout << input_symbol;
            std::getline(std::cin, line);
            return line;
        }
    }

    /**
     * Contains arrays actions
     */
    export namespace array {
        /**
         * Delete dynamically allocated array
         * @tparam T generic type.
         * @param array array of generic type.
         * @param rows rows count in this array.
         */
        template<typename T>
        void deleteDynamicArray(T *array, const int rows) {
            for (int i = 0; i < rows; ++i) {
                delete[] array[i];
            }
            delete[] array;
        }

        template<typename T>
        T *create1DArray(int);

        template<typename T>
        T *create1DArray(const int rows) {
            const auto dyn_array = new T[rows];
            for (int i = 0; i < rows; ++i) {
                dyn_array[i] = 0;
            }
            return dyn_array;
        }

        /**
         * Create one dimensional array of generic type
         * @param rows rows count
         * @return constructed array of generic type
         */
        template<>
        std::string *create1DArray(const int rows) {
            const auto dyn_array = new std::string[rows];
            for (int i = 0; i < rows; ++i) {
                dyn_array[i] = "";
            }
            return dyn_array;
        }

        /**
         * Inline function for creating 2d generic array
         * @param rows rows of the array
         * @param cols columns of the array
         * @tparam T generic param for type of the objects in array
         * @return: initialized 2d generic array
         */
        template<typename T>
        T **create2DArray(const int rows, const int cols) {
            const auto dyn_array = new T *[rows];
            for (int i = 0; i < rows; ++i) {
                dyn_array[i] = new T *[cols];
            }
            return dyn_array;
        }

        /**
         * @tparam T generic type
         * @param depth
         * @param sizes
         * @return
         */
        template<typename T>
        std::vector<void *> createNDimArray(const size_t depth, const std::vector<size_t> &sizes) {
            if (depth == sizes.size()) {
                return {};
            }
            std::vector<std::vector<void *> > result(sizes[depth]);
            for (auto &sub: result) {
                sub = createNDimArray<T>(depth + 1, sizes);
            }
            return static_cast<std::vector<T>>(result);
        }

        /**
         * Resolve old string (c-style) string and return its size.
         * @param old_string c-style string
         * @return int value of size
         */
        template<typename T>
        int get_dynamic_array_size([[maybe_unused]] T *old_string) {
            constexpr int count = sizeof(old_string) / sizeof(old_string[0]);
            return count;
        }

#ifdef LIBIO_EXPERIMENTAL
        /**
         *
         * @tparam T generic type
         * @param sizes
         * @return
         */
        template<typename T>
        std::vector<T> createNDimArray(const std::vector<size_t> &sizes) {
            if (sizes.empty())
                return std::vector<T>();

            std::vector<T> flat;
            if (sizes.size() == 1) {
                return std::vector<T>(sizes[0]);
            }
            if (sizes.size() == 2) {
                std::vector<std::vector<T> > result(sizes[0], std::vector<T>(sizes[1]));
                return result;
            }
            return;
        }

        std::tuple<int *, int, int> increase_dynamic_array(int *arr, int logical_size, int actual_size) {
            if (arr != nullptr) {
                actual_size *= 2;
                auto new_arr = new int[actual_size];
                for (int i = 0; i < logical_size; ++i) {
                    new_arr[i] = arr[i];
                }
                delete[] arr;
                return {new_arr, logical_size, actual_size};
            }
            std::cerr << "Ошибка! Невозможно выделить дополнительную память для массива" << "\n";
            throw;
        }
#endif
    }
    /**
     * Contains file actions.
     * Ex. write or create.
     */
    export namespace file {
        /**
         * Creates file for read and write.
         * @param fileName name of the file, create if not exists.
         * @return file handler.
         */
        inline std::ofstream createWriteFile(const std::string &fileName) {
            std::ofstream file(fileName);
            return file;
        }

        /**
         * Read file line by line.
         * @param fileName name of the file.
         * @return vector with lines.
         */
        inline std::vector<std::string> readFile(const std::string &fileName) {
            auto lines = std::vector<std::string>();
            if (std::ifstream file(fileName); file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    lines.emplace_back(line);
                }
                file.close();
                return lines;
            }
            std::cerr << "Error reading file " << fileName << std::endl;
            return {};
        }

        /**
         * Write file line by line.
         * @param fileName name of the file to write into.
         * @param lines vector value of lines of text.
         * @return output file handler.
         */
        inline std::ofstream writeFile(const std::string &fileName, const std::vector<std::string> &lines) {
            auto out = std::ofstream(fileName);
            std::for_each(lines.begin(), lines.end(), [&out](const std::string &line) {
                out << line << std::endl;
            });
            return out;
        }

#ifdef LIBIO_EXPERIMENTAL
            /**
             * Function for receiving few lines from file.
             * @tparam T generic type.
             * @param fileName name of the file.
             * @param count how many lines to get.
             * @return vector with string values.
             */
            template<typename T>
            std::vector<T> getFewLinesFrom(const std::string &fileName, const int count) {
                std::ifstream file(fileName);
                auto lines = std::vector<T>();
                int inner_counter = 0;
                std::string line;
                while (std::getline(file, line)) {
                    ++inner_counter;
                    if (inner_counter == count) {
                        break;
                    }
                    lines.emplace_back(line);
                }
                return lines;
            }

            /**
             * Platform independent filepath getter.
             * @deprecated because crashes program due to strange path get.
             * @return string value of current path
             */
            inline std::string getCwd() {
                const std::filesystem::path currentPath = std::filesystem::current_path();
                return currentPath.string();
            }

#endif
    }

    /**
     * Namespace for concurrency things
     */
    namespace concurrency {
        //
    }

    /**
     * Namespace for database tricks
     */
    namespace database {
        enum DATABASE_TYPE {
            //
        };
#ifdef LIBIO_EXPERIMENTAL
        void create_connection(const std::string& database_name) {
            //
        }

        void close_connection(const std::string& database_name) {
            //
        }
#endif
    }
}
