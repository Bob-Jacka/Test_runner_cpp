#include "../declaration/Check_list.hpp"

TA::Check_list::Check_list(const std::string &name, const std::string &description) {
    this->name = name;
    this->description = description;
    this->steps = std::vector<std::string>();
}

TA::Check_list::Check_list(const std::string &name, const std::string &description, const std::vector<std::string> &steps) {
    this->name = name;
    this->description = description;
    this->steps = steps;
}

std::string TA::Check_list::get_name() const {
    return this->name;
}

std::vector<std::string> TA::Check_list::get_steps() const {
    return this->steps;
}

std::string TA::Check_list::to_string() const {
    return "";
}

std::string TA::Check_list::get_description() const {
    return this->description;
}
