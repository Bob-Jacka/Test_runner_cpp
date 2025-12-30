#ifndef TESTSUIT_HPP
#define TESTSUIT_HPP

#include "Test_artifact.hpp"

namespace Check_runner {
    namespace TA {
        /**
         * Class for combine test artifacts.
         * @tparam T generic type for test artifact in suit.
         */
        template<typename T>
            requires std::is_base_of_v<Test_artifact, T>
        class Test_suit final : public Test_artifact {
            using Dummy_map = std::vector<std::pair<std::string, std::string> >; //it will be error if you add map header
            std::string suit_name; ///name of the suit
            Dummy_map local_parameters; ///local parameters of the suit
            std::vector<T> test_artifacts; ///vector with generic test artifacts

        public:
            Test_suit() = delete;

            explicit Test_suit(const std::string &suit_name) {
                this->suit_name = suit_name;
                this->test_artifacts = std::vector<T>();
                this->local_parameters = Dummy_map();
            }

            Test_suit(const std::string &suit_name, std::vector<T> &tests) {
                this->suit_name = suit_name;
                this->test_artifacts = tests;
                this->local_parameters = Dummy_map();
            }

            ~Test_suit() override {
                this->delete_test_artifacts();
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

            void delete_test_artifacts() {
                this->test_artifacts.clear();
            }

            /**
             * Add local parameter to suit.
             * @param key name of the parameter
             * @param value value of the parameter
             */
            void add_local_parameter(const std::string &key, const std::string &value) {
                for (auto &dummy_map_pair: this->local_parameters) {
                    if (dummy_map_pair.first == key) {
                        dummy_map_pair.second = value;
                        break;
                    }
                }
                throw Check_exceptions::TestArtifactException(__LINE__, "No valid key found in local parameters of suit", __FILE_NAME__);
            }
        };
    }
}

#endif
