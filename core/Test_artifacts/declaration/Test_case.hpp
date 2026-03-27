#ifndef TEST_CASE_HPP
#define TEST_CASE_HPP

#include "Test_artifact.hpp"

namespace Check_runner {
    namespace TA {
        /**
         * Data class for test case
         */
        class Test_case final : public Test_artifact {
            std::string              name;
            std::string              comment; ///comment or description in test case
            std::string              tc_hash;
            std::vector<std::string> steps; ///steps of the test case to execute

            TA_helper_data::Priority priority;
            TA_helper_data::Severity severity; //TODO useless parameter, delete later

            public:
                Test_case() = delete;

                Test_case(const Test_case &);

                Test_case( Test_case &&) noexcept;

                Test_case(const std::string &, const std::string &, TA_helper_data::Priority,
                          TA_helper_data::Severity = TA_helper_data::Severity::Medium);

                ~Test_case() override = default;

                //operators

                bool operator<(const Test_case &rhs) const;

                bool operator<=(const Test_case &rhs) const;

                bool operator>(const Test_case &rhs) const;

                bool operator>=(const Test_case &rhs) const;

                bool operator==(const Test_case &rhs) const;

                bool operator!=(const Test_case &rhs) const;

                Test_case &operator=(const Test_case &);

                //methods

                [[nodiscard]] std::string get_name() const override;

                [[nodiscard]] std::string get_comment() const;

                [[nodiscard]] TA_helper_data::Priority get_priority() const;

                [[nodiscard]] TA_helper_data::Severity get_severity() const;

                [[nodiscard]] std::vector<std::string> get_steps() const;

                [[nodiscard]] std::string to_string() const override;

                [[nodiscard]] std::string get_hash() const;

                void set_hash(const std::string &);

                friend void swap(Test_case &lhs, Test_case &rhs) noexcept;
        };
    }
}
#endif
