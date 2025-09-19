#ifndef FILEREADER_HPP
#define FILEREADER_HPP
#include <fstream>
#include <string>
#include <vector>

class File_controller {
    std::string file_name;
    std::ofstream input_file;

public:
    explicit File_controller(const std::string &file_name) : file_name(file_name) {
        input_file = std::ofstream(file_name);
    }

    ~File_controller() = default;

    //non-static

    std::vector<std::string> readlines();

    std::string readline();

    //static methods, open file and read

    static std::vector<std::string> readlines(const std::string &);

    static std::vector<std::string> readlines(std::ofstream &);

    //static read one line from file and return it

    static std::string readline(std::string &);

    static std::string readline(std::ofstream &);

    //create file

    void open_file(const std::string &);

    //other

    void close_file(const std::string &);

    File_controller() = delete;

    File_controller(const File_controller &) = delete;

    File_controller(File_controller &&) = delete;

    File_controller &operator=(const File_controller &) = delete;

    File_controller &operator=(File_controller &&) = delete;

    //Test result document file

    std::fstream create_test_result_file(); //for test results output
};

#endif
