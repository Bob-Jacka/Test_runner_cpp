#include "../declaration/Bug.hpp"

std::string TA::Bug::get_name() const {
    return this->name;
}

TA::Bug::Bug(const std::string &name, const std::string &description, const Severity severity) {
    this->name = name;
    this->description = description;
    this->severity = severity;
}

std::string TA::Bug::to_string() const {
    return "";
}

std::string TA::Bug::get_description() const {
    return this->description;
}

TA::Severity TA::Bug::get_severity() const {
    return this->severity;
}

TA::Bug TA::Bug::operator<=>(Bug &other) const {
    //
}
