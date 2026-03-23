#ifndef FILEREADER_HPP
#define FILEREADER_HPP

//File controller dependencies:
#include <utility>
#include <vector>

#include "../Exceptions/FileControllerException.hpp"

namespace Check_runner {
    constexpr auto TXT_F = ".txt"; //simple old txt files
#ifdef NEW_FILE_FORMAT
    constexpr auto RSF_F = ".rsf"; //brave new file format for test results
    constexpr auto TF    = ".tf";  //new format for test files
#endif
    /**
     * Class for fs actions;
     * There are two special files in utility:
     * .rsf (for test results)
     * .*tf (for tests)
     */
    class File_controller {
        public:
            File_controller() = default;

            ~File_controller() = default;

            //static methods, open file and read data from it

            static std::vector<std::string> readlines(const std::string &, int);

            static std::vector<std::string> readlines(const std::string &);

            static std::vector<std::string> readlines(std::ifstream &, int);

            //static read one line from file and return it

            static std::string readline(const std::string &);

            static std::string readline(std::ifstream &);

            File_controller(const File_controller &) = delete;

            File_controller(File_controller &&) = delete;

            File_controller &operator=(const File_controller &) = delete;

            File_controller &operator=(File_controller &&) = delete;

            ///for test results output. @deprecated due to new file format

            static int check_file_extension(const std::string &);

            static bool check_file_existence(std::string &);

            /**
            * Open file and return condition variable of open
            * @param file_name name of the file to open
            * @tparam T generic type (which type of file to create)
            * @return tuple with file handler and bool (true if file open or error)
            */
            template<typename T>
                requires std::is_same_v<T, std::fstream> || std::is_same_v<T, std::ifstream>
            static std::tuple<T, bool> open_file(const std::string &file_name) {
                if (auto file = T(file_name); file.is_open()) {
                    return std::make_tuple<T, bool>(std::move(file), true);
                }
                return std::make_tuple(T(), false); //File not exist with name
            }

#ifdef NEW_FILE_FORMAT
            static std::tuple<std::ifstream, bool> create_tf_file(const std::string &file_name); ///file for tests
            static std::tuple<std::ifstream, bool> create_rsf_file(const std::string &file_name); ///file for test results
#endif
            //Test result document file
            static std::fstream create_test_result_file(const std::string & = "results.txt");
    };
}
#endif
