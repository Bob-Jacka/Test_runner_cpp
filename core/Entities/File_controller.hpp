#ifndef FILEREADER_HPP
#define FILEREADER_HPP

//File controller dependencies:
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <tuple>

namespace Check_runner {
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

        static std::fstream create_test_result_file(const std::string & = "results.txt"); //for test results output

        static int check_file_extension(const std::string &);

        static bool check_file_existence(std::string &);

        static std::tuple<std::ifstream, bool> open_file(const std::string &);
    };
}
#endif
