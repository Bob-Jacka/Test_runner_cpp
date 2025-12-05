#include "../declaration/Test_result.hpp"

std::string Check_runner::TA::Test_result::get_name() const {
    return this->name;
}

std::string Check_runner::TA::Test_result::to_string() const {
    return "";
}

std::string Check_runner::TA::Test_result::get_result() const {
    return this->result;
}

void Check_runner::TA::Test_result::add_bug(const Bug &bug) {
    this->bugs.push_back(bug);
}

std::vector<Check_runner::TA::Bug> Check_runner::TA::Test_result::get_bugs() const {
    return this->bugs;
}

std::string Check_runner::TA::Test_result::get_device_name() const {
    return this->device_name;
}

void Check_runner::TA::Test_result::set_bugs(const std::vector<Bug> &bugs) {
    this->bugs = bugs;
}

void Check_runner::TA::Test_result::set_device_name(const std::string &device_name) {
    this->device_name = device_name;
}

void Check_runner::TA::Test_result::set_name(const std::string &name) {
    this->name = name;
}

void Check_runner::TA::Test_result::set_result(const std::string &result) {
    this->result = result;
}

Check_runner::TA::Test_result::Test_result(const std::string &name, const std::string &result) {
    this->name = name;
    this->result = result;
}

Check_runner::TA::Test_result::Test_result(const std::string &name, const std::string &result, const std::vector<Bug> &bugs) {
    this->name = name;
    this->result = result;
    this->bugs = bugs;
}

Check_runner::TA::Test_result::Test_result() {
    this->name = "";
    this->result = "";
    this->bugs = std::vector<Bug>();
}
