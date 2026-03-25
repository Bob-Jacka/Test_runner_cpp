#include "../declaration/Bug.hpp"

/**
 * Main bug constructor
 * @param name name of the bug
 * @param description description of the bug
 * @param severity severity of the bug
 */
Check_runner::TA::Bug::Bug(const std::string &name, const std::string &description, const TA_helper_data::Severity severity) {
    if (!name.empty() and !description.empty()) {
        this->name        = name;
        this->description = description;
        this->severity    = severity;
    } else {
        throw Check_exceptions::TestArtifactException(__LINE__, "Name or description should not be empty",
                                                      __FILE_NAME__);
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

Check_runner::TA_helper_data::Severity Check_runner::TA::Bug::get_severity() const {
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

Check_runner::TA::Question_or_enhance::Question_or_enhance() {
    this->name        = "";
    this->description = "";
}

////Question
Check_runner::TA::Question_or_enhance::Question_or_enhance(const std::string &name, const std::string &description,
                                                           const Type         type) {
    this->name        = name;
    this->description = description;
    this->type        = type;
}

std::string Check_runner::TA::Question_or_enhance::get_name() const {
    return this->name;
}

std::string Check_runner::TA::Question_or_enhance::to_string() const {
    return "";
}

std::string Check_runner::TA::Question_or_enhance::get_description() const {
    return this->description;
}

Check_runner::TA::Type Check_runner::TA::Question_or_enhance::get_type() const {
    return this->type;
}

void Check_runner::TA::enter_question_or_enhance(Question_or_enhance &object) {
    if (std::cin.good()) {
        std::cout << "Enter name: ";
        std::cin >> object.name;
        std::cout << "\n";

        std::cout << "Enter description: ";
        std::cin >> object.description;
        std::cout << "\n";
    } else {
        throw;
    }
}

std::string Check_runner::TA::convert_type_2str(const Question_or_enhance &object) {
    switch (object.get_type()) {
        case Type::QUESTION:
            return "Question";
        case Type::ENHANCE:
            return "Enhance";
        default: throw;
    }
}

Check_runner::TA::Type Check_runner::TA::convert_str_2type(const std::string &object) {
    if (object == "Question") {
        return Type::QUESTION;
    } else if (object == "Enhance") {
        return Type::ENHANCE;
    } else {
        throw;
    }
}
