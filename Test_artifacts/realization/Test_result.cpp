#include "../declaration/Test_result.hpp"

std::string TA::Test_result::get_name() const {
    return this->name;
}

std::string TA::Test_result::to_string() const {
    return "";
}

std::string TA::Test_result::get_result() const {
    return this->result;
}

void TA::Test_result::add_bug(const Bug &bug) {
    this->bugs.push_back(bug);
}

std::vector<TA::Bug> TA::Test_result::get_bugs() const {
    return this->bugs;
}

std::string TA::Test_result::get_device_name() const {
    return this->device_name;
}

void TA::Test_result::set_bugs(const std::vector<Bug> &bugs) {
    this->bugs = bugs;
}

void TA::Test_result::set_device_name(const std::string &device_name) {
    this->device_name = device_name;
}

void TA::Test_result::set_name(const std::string &name) {
    this->name = name;
}

void TA::Test_result::set_result(const std::string &result) {
    this->result = result;
}

TA::Test_result::Test_result(const std::string &name, const std::string &result) {
    this->name = name;
    this->result = result;
}

TA::Test_result::Test_result(const std::string &name, const std::string &result, const std::vector<Bug> &bugs) {
    this->name = name;
    this->result = result;
    this->bugs = bugs;
}

TA::Test_result::Test_result() {
    this->name = "";
    this->result = "";
    this->bugs = std::vector<Bug>();
}
