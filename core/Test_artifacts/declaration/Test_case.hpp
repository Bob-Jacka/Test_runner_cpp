#ifndef TEST_CASE_HPP
#define TEST_CASE_HPP

#include "Test_artifact.hpp"

namespace Check_runner {
    namespace TA {
        /**
         * Data class for test case
         */
        class Test_case final : public Test_artifact {
            std::string name;
            std::string comment; ///comment or description in test case
            std::vector<std::string> steps; ///steps of the test case to execute

            Priority priority;
            Severity severity;

        public:
            Test_case() = delete;

            Test_case(const Test_case &);

            Test_case(const std::string &,
                      const std::string &,
                      Priority,
                      Severity = Severity::Medium);

            ~Test_case() override = default;

            //operators

            bool operator<(const Test_case &rhs) const;

            bool operator<=(const Test_case &rhs) const;

            bool operator>(const Test_case &rhs) const;

            bool operator>=(const Test_case &rhs) const;

            bool operator==(const Test_case &rhs) const;

            bool operator!=(const Test_case &rhs) const;

            Test_case &operator=(const Test_case &) = delete;

            //methods

            [[nodiscard]] std::string get_name() const override;

            [[nodiscard]] std::string get_comment() const;

            [[nodiscard]] Priority get_priority() const;

            [[nodiscard]] Severity get_severity() const;

            [[nodiscard]] std::vector<std::string> get_steps() const;

            [[nodiscard]] std::string to_string() const override;

            friend void swap(Test_case &lhs, Test_case &rhs) noexcept;
        };
    }
}
#endif
