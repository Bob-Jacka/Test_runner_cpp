#ifndef TEST_CASE_HPP
#define TEST_CASE_HPP

#include "TestArtifact.hpp"

namespace TA {
    /**
     * Data class for test case
     */
    class Test_case final : public TestArtifact {
        std::string name;
        std::string comment;
        std::vector<std::string> steps; ///steps of the test case to execute

        Priority priority;
        Severity severity;

    public:
        Test_case() = delete;

        Test_case(std::string,
                  std::string,
                  Priority,
                  Severity = Severity::Medium);

        Test_case(Test_case const &) = delete;

        ~Test_case() override = default;

        //operators

        Test_case operator<=>() const;

        Test_case &operator=(const Test_case &) = delete;

        //methods

        [[nodiscard]] std::string get_name() const override;

        [[nodiscard]] std::string get_comment() const;

        [[nodiscard]] Priority get_priority() const;

        [[nodiscard]] Severity get_severity() const;

        void to_string() const override;
    };
}
#endif
