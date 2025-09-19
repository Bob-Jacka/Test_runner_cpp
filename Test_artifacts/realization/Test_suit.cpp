#include "../declaration/Test_suit.hpp"

template<typename T> requires std::derived_from<TA::TestArtifact, T>
Test_suit<T>::Test_suit(std::string suit_name) {
    //
}

template<typename T> requires std::derived_from<TA::TestArtifact, T>
Test_suit<T>::Test_suit(std::string, std::pmr::vector<T>) {
    //
}

template<typename T> requires std::derived_from<TA::TestArtifact, T>
Test_suit<T>::~Test_suit() {
    delete_test_artifacts();
}

template<typename T> requires std::derived_from<TA::TestArtifact, T>
void Test_suit<T>::count_test_artifacts() const {
    int counter = 0;
    for (register const auto &elem: this->test_artifacts) {
        counter++;
    }
}

template<typename T> requires std::derived_from<TA::TestArtifact, T>
void Test_suit<T>::add_test_artifacts(TestArtifact *other) {
    this->test_artifacts.push_back(other);
}

template<typename T> requires std::derived_from<TA::TestArtifact, T>
void Test_suit<T>::delete_test_artifacts(std::string &other) {
    this->test_artifacts.at(other) = nullptr;
}

template<typename T> requires std::derived_from<TA::TestArtifact, T>
void ::Test_suit<T>::to_string() const {
    //
}

template<typename T> requires std::derived_from<TA::TestArtifact, T>
std::string Test_suit<T>::get_name() const {
    return suit_name;
}
