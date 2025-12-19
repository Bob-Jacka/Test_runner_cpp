#include "../declaration/Bug.hpp"

/**
 * Main bug constructor
 * @param name name of the bug
 * @param description description of the bug
 * @param severity
 */
Check_runner::TA::Bug::Bug(const std::string &name, const std::string &description, const Severity severity) {
    if (!name.empty() and !description.empty()) {
        this->name = name;
        this->description = description;
        this->severity = severity;
    } else {
        throw Check_exceptions::TestArtifactException(__LINE__, "Name or description should not be empty", __FILE_NAME__);
    }
}

std::string Check_runner::TA::Bug::get_name() const {
    return this->name;
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

bool Check_runner::TA::Bug::operator<(const Bug &rhs) const {
    if (this->name < rhs.name)
        return true;
    if (rhs.name < this->name)
        return false;
    if (this->description < rhs.description)
        return true;
    if (rhs.description < this->description)
        return false;
    return this->severity < rhs.severity;
}

bool Check_runner::TA::Bug::operator<=(const Bug &rhs) const {
    return rhs.severity >= this->severity;
}

bool Check_runner::TA::Bug::operator>(const Bug &rhs) const {
    return rhs.severity < this->severity;
}

bool Check_runner::TA::Bug::operator>=(const Bug &rhs) const {
    return this->severity >= rhs.severity;
}
