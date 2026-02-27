#ifndef FILEREADER_HPP
#define FILEREADER_HPP

//File controller dependencies:
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "../Exceptions/FileControllerException.hpp"

#define TXT_F ".txt" //simple old txt files
#define RSF_F ".rsf" //brave new file format for test results
#define TF ".tf" //new format for test files

namespace Check_runner {
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

            //Test result document file

            static std::fstream create_test_result_file(const std::string & = "results.txt");

            ///for test results output. @deprecated due to new file format

            static int check_file_extension(const std::string &);

            static bool check_file_existence(std::string &);

            static std::tuple<std::ifstream, bool> open_file(const std::string &);

#ifdef NEW_FILE_FORMAT
            static std::tuple<std::ifstream, bool> create_tf_file(const std::string &file_name); ///file for tests

            static std::tuple<std::ifstream, bool> create_rsf_file(const std::string &file_name); ///file for test results
#endif
    };
}
#endif
