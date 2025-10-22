#ifndef TESTCASEFABRIC_HPP
#define TESTCASEFABRIC_HPP

#include "../Data/Constants.hpp"
#include "../Test_artifacts/declaration/Bug.hpp"
#include "../Test_artifacts/declaration/Check_list.hpp"
#include "../Test_artifacts/declaration/Test_case.hpp"
#include "../Test_artifacts/declaration/Test_suit.hpp"

import UtilFuncs_mod;

/**
 * Styles for decompose test cases
 */
enum class TS_style {
    TXT, //style for simple txt notepad
    GOOGLE_STYLESHEET,
    TEST_RAIL,
    JIRA,
    XML,
    JSON
};

/**
 * Entity for creating test cases.
 * Also support decompose to other formats.
 */
class TestCaseFabric {
public:
    TestCaseFabric();

    ~TestCaseFabric();

    TestCaseFabric(const TestCaseFabric &) = delete;

    TestCaseFabric &operator=(const TestCaseFabric &) = delete;

    //Methods for creating test artifacts

    [[nodiscard]] std::vector<TA::Test_case> create_test_cases(const std::vector<std::string> &) const;

    [[nodiscard]] TA::Test_case create_test_case(const std::string &, const std::string &, const TA::Priority &) const;

    [[nodiscard]] TA::Bug create_bug(const std::string &, const std::string &) const;

    [[nodiscard]] TA::Check_list create_check_list(const std::string &, const std::string &, const std::vector<std::string> &) const;

    [[nodiscard]] TA::Test_suit<TA::Test_case> create_test_suit(const std::vector<std::string> &, const std::string &);

    //Other actions

    void delete_test_case() const;

    [[nodiscard]] std::string decompose_test_case(TS_style) const;
};

#endif
