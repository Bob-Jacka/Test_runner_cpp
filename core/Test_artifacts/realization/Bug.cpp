#include "../declaration/Bug.hpp"

std::string Check_runner::TA::Bug::get_name() const {
    return this->name;
}

Check_runner::TA::Bug::Bug(const std::string &name, const std::string &description, const Severity severity) {
    this->name = name;
    this->description = description;
    this->severity = severity;
}

std::string Check_runner::TA::Bug::to_string() const {
    return "";
}

std::string Check_runner::TA::Bug::get_description() const {
    return this->description;
}

Check_runner::TA::Severity Check_runner::TA::Bug::get_severity() const {
    return this->severity;
}

Check_runner::TA::Bug Check_runner::TA::Bug::operator<=>(Bug &other) const {
    //
}
