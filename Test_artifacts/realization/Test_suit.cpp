#include "../declaration/Test_suit.hpp"

template<typename T>
TA::Test_suit<T>::Test_suit(const std::string &suit_name) {
    this->suit_name = suit_name;
}

template<typename T>
TA::Test_suit<T>::Test_suit(const std::string &suit_name, std::vector<T> &tests) {
    this->suit_name = suit_name;
    this->test_artifacts = tests;
}

template<typename T>
TA::Test_suit<T>::~Test_suit() {
    delete_test_artifacts();
}

template<typename T>
int TA::Test_suit<T>::count_test_artifacts() const {
    return this->test_artifacts.size();
}

template<typename T>
void TA::Test_suit<T>::add_test_artifacts(TA::Test_artifact *other) {
    this->test_artifacts.push_back(other);
}

template<typename T>
void TA::Test_suit<T>::delete_test_artifacts(std::string &other) {
    this->test_artifacts.at(other) = nullptr;
}

template<typename T>
std::string TA::Test_suit<T>::to_string() const {
    return "";
}

template<typename T>
std::string TA::Test_suit<T>::get_name() const {
    return suit_name;
}
