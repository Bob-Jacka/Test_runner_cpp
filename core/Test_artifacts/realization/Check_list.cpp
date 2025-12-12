#include "../declaration/Check_list.hpp"

Check_runner::TA::Check_list::Check_list(const std::string &name, const std::string &description) {
    if (!name.empty() and !description.empty()) {
        this->name = name;
        this->description = description;
        this->steps = std::vector<std::string>();
    } else {
        throw Check_exceptions::TestArtifactException("Name or description cannot be empty string");
    }
}

Check_runner::TA::Check_list::Check_list(const std::string &name, const std::string &description, const std::vector<std::string> &steps) {
    if (!name.empty() and !description.empty()) {
        this->name = name;
        this->description = description;
        this->steps = steps;
    } else {
        throw Check_exceptions::TestArtifactException("Name or description cannot be empty string");
    }
}

std::string Check_runner::TA::Check_list::get_name() const {
    return this->name;
}

std::vector<std::string> Check_runner::TA::Check_list::get_steps() const {
    return this->steps;
}

std::string Check_runner::TA::Check_list::to_string() const {
    return "";
}

std::string Check_runner::TA::Check_list::get_description() const {
    return this->description;
}
