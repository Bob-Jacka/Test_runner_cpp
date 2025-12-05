#ifndef TESTSUIT_HPP
#define TESTSUIT_HPP

#include "Test_artifact.hpp"

namespace Check_runner {
    namespace TA {
        /**
         * Class for combine test artifacts.
         * @tparam T generic type for test artifact.
         */
        template<typename T>
            requires std::is_base_of_v<Test_artifact, T>
        class Test_suit final : public Test_artifact {
            std::string suit_name; ///name of the suit
            std::vector<T> test_artifacts; ///vector with generic test artifacts

        public:
            ~Test_suit() override {
                delete_test_artifacts();
            }

            Test_suit() = delete;

            explicit Test_suit(const std::string &suit_name) {
                this->suit_name = suit_name;
            }

            Test_suit(const std::string &suit_name, std::vector<T> &tests) {
                this->suit_name = suit_name;
                this->test_artifacts = tests;
            }

            [[nodiscard]] std::string get_name() const override {
                return suit_name;
            }

            [[nodiscard]] std::string to_string() const override {
                return "";
            }

            [[nodiscard]] int count_test_artifacts() const {
                return this->test_artifacts.size();
            }

            void add_test_artifacts(T *other) {
                this->test_artifacts.push_back(other);
            }

            void delete_test_artifacts(std::string &other) {
                this->test_artifacts.at(other) = nullptr;
            }
        };
    }
}

#endif
