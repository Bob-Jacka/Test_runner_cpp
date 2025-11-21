#include "../declaration/Test_case.hpp"

std::string TA::Test_case::get_comment() const {
    return this->comment;
}

TA::Priority TA::Test_case::get_priority() const {
    return this->priority;
}

TA::Test_case::Test_case(const Test_case &other) : Test_artifact(other) {
    this->name = other.name;
    this->severity = other.severity;
    this->steps = other.steps;
    this->comment = other.comment;
    this->priority = other.priority;
}

TA::Test_case::Test_case(const std::string &name, const std::string &comment, const Priority given_prior, const Severity given_sev) {
    this->name = name;
    this->comment = comment;
    this->priority = given_prior;
    this->severity = given_sev;
}

bool TA::Test_case::operator<(const Test_case &rhs) const {
    if (this->name < rhs.name)
        return true;
    if (rhs.name < this->name)
        return false;
    if (this->comment < rhs.comment)
        return true;
    if (rhs.comment < this->comment)
        return false;
    if (this->steps < rhs.steps)
        return true;
    if (rhs.steps < this->steps)
        return false;
    if (this->priority < rhs.priority)
        return true;
    if (rhs.priority < this->priority)
        return false;
    return this->severity < rhs.severity;
}

bool TA::Test_case::operator<=(const Test_case &rhs) const {
    return rhs.severity >= severity;
}

bool TA::Test_case::operator>(const Test_case &rhs) const {
    return rhs.severity < severity;
}

bool TA::Test_case::operator>=(const Test_case &rhs) const {
    return severity >= rhs.severity;
}

bool TA::Test_case::operator==(const Test_case &rhs) const {
    return name == rhs.name
           && comment == rhs.comment
           && steps == rhs.steps
           && priority == rhs.priority
           && severity == rhs.severity;
}

bool TA::Test_case::operator!=(const Test_case &rhs) const {
    return severity != rhs.severity;
}

std::string TA::Test_case::get_name() const {
    return this->name;
}

TA::Severity TA::Test_case::get_severity() const {
    return this->severity;
}

std::vector<std::string> TA::Test_case::get_steps() const {
    return this->steps;
}

std::string TA::Test_case::to_string() const {
    return "";
}

void TA::swap(Test_case &lhs, Test_case &rhs) noexcept {
    std::swap(lhs.name, rhs.name);
    std::swap(lhs.severity, rhs.severity);
    std::swap(lhs.steps, rhs.steps);
    std::swap(lhs.comment, rhs.comment);
    std::swap(lhs.priority, rhs.priority);
}
