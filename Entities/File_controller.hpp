#ifndef FILEREADER_HPP
#define FILEREADER_HPP

//File controller dependencies:
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class File_controller {
    std::string file_name;
    std::unique_ptr<std::ofstream> input_file;
    std::unique_ptr<std::ofstream> output_file;

public:
    explicit File_controller(std::string file_name) : file_name(std::move(file_name)) {
        input_file = std::make_unique<std::ofstream>();
    }

    ~File_controller() = default;

    //non-static

    std::vector<std::string> &readlines();

    std::string readline();

    //static methods, open file and read

    static std::vector<std::string> readlines(const std::string &, int);

    static std::vector<std::string> readlines(std::ofstream &, int);

    //static read one line from file and return it

    static std::string readline(const std::string &);

    static std::string readline(std::ofstream &);

    //create file

    void open_file(const std::string &) const;

    //other

    void close_file() const;

    File_controller() = delete;

    File_controller(const File_controller &) = delete;

    File_controller(File_controller &&) = delete;

    File_controller &operator=(const File_controller &) = delete;

    File_controller &operator=(File_controller &&) = delete;

    //Test result document file

    std::fstream create_test_result_file(std::string = "results.txt"); //for test results output
};

#endif
