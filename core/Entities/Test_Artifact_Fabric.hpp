#ifndef TESTCASEFABRIC_HPP
#define TESTCASEFABRIC_HPP

#include "../Data/Constants.hpp"
#include "../Test_artifacts/declaration/Bug.hpp"
#include "../Test_artifacts/declaration/Check_list.hpp"
#include "../Test_artifacts/declaration/Test_case.hpp"
#include "../Test_artifacts/declaration/Test_suit.hpp"

namespace Check_runner {
    /**
     * Namespace for all test artifacts in testing
     */
    namespace TA {
        /**
         * Styles for decompose test cases
         */
        enum class TS_style {
            TXT, //style for simple txt notepad
            GOOGLE_STYLESHEET, //Special for Google tables
            TEST_RAIL,
            JIRA, //Confluence style
            XML,
            JSON
        };

        /**
         * Entity for creating test cases.
         * Also support decompose to other formats.
         */
        class Test_artifact_fabric {
        public:
            Test_artifact_fabric();

            ~Test_artifact_fabric();

            Test_artifact_fabric(const Test_artifact_fabric &) = delete;

            Test_artifact_fabric &operator=(const Test_artifact_fabric &) = delete;

            //Methods for creating test artifacts

            [[nodiscard]] std::vector<Check_runner::TA::Test_case> &create_test_cases(const std::vector<std::string> &) const;

            [[nodiscard]] Bug *create_bug(const std::string &, const std::string &) const;

            [[nodiscard]] Check_list *create_check_list(const std::string &, const std::string &,
                                                        const std::vector<std::string> &) const;

            [[nodiscard]] Test_suit<Test_case> *create_test_suit(
                const std::vector<std::string> &, const std::string &) const;

            //Other actions

            [[nodiscard]] std::string decompose_test_case(const Test_case &, TS_style) const;
        };
    }
}
#endif
