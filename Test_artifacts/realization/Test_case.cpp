#include "../declaration/Test_case.hpp"

std::string TA::Test_case::get_comment() const {
    return this->comment;
}

TA::Priority TA::Test_case::get_priority() const {
    return this->priority;
}

TA::Test_case::Test_case(const std::string &name, const std::string &comment, const Priority given_prior, const Severity given_sev) {
    this->name = name;
    this->comment = comment;
    this->priority = given_prior;
    this->severity = given_sev;
}

std::string TA::Test_case::get_name() const {
    return this->name;
}

TA::Severity TA::Test_case::get_severity() const {
    return this->severity;
}

std::string TA::Test_case::to_string() const {
    return "";
}

TA::Test_case TA::Test_case::operator<=>(Test_case &other) const {
    //
}
