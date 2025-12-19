#include "../declaration/Test_result.hpp"

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

std::string Check_runner::TA::Test_result::to_string() const {
    return "";
}

std::string Check_runner::TA::Test_result::get_name() const {
    return this->name;
}

std::string Check_runner::TA::Test_result::get_result() const {
    return this->result;
}

std::vector<Check_runner::TA::Bug> Check_runner::TA::Test_result::get_bugs() const {
    return this->bugs;
}

std::string Check_runner::TA::Test_result::get_device_name() const {
    return this->device_name;
}

void Check_runner::TA::Test_result::add_bug(const Bug &bug) {
    if (!bug.get_name().empty() and !bug.get_description().empty()) {
        this->bugs.push_back(bug);
    } else {
        throw Check_exceptions::TestArtifactException(__LINE__, "bug name cannot be empty string", __FILE_NAME__);
    }
}

void Check_runner::TA::Test_result::set_bugs(const std::vector<Bug> &bugs) {
    this->bugs = bugs;
}

void Check_runner::TA::Test_result::set_device_name(const std::string &device_name) {
    if (!device_name.empty()) {
        this->device_name = device_name;
    } else {
        throw Check_exceptions::TestArtifactException(__LINE__, "device name cannot be empty string", __FILE_NAME__);
    }
}

void Check_runner::TA::Test_result::set_name(const std::string &name) {
    if (!name.empty()) {
        this->name = name;
    } else {
        throw Check_exceptions::TestArtifactException(__LINE__, "name cannot be empty string", __FILE_NAME__);
    }
}

void Check_runner::TA::Test_result::set_result(const std::string &result) {
    if (!result.empty()) {
        this->result = result;
    } else {
        throw Check_exceptions::TestArtifactException(__LINE__, "result cannot be empty string", __FILE_NAME__);
    }
}
