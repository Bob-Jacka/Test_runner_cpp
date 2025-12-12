#include <stdexcept>

#include "Test_Artifact_Fabric.hpp"

Test_artifact_fabric::Test_artifact_fabric() = default;

Test_artifact_fabric::~Test_artifact_fabric() = default;

/**
 * Create test cases with use of given vector strings to create.
 * @param string_lines string lines to create test cases
 * @return vector with test cases, instead of strings
 */
std::vector<Check_runner::TA::Test_case> &Test_artifact_fabric::create_test_cases(const std::vector<std::string> &string_lines) const {
    if (!string_lines.empty()) {
        auto tmp_test_cases = new std::vector<Check_runner::TA::Test_case>();
        for (auto &test_case_line: string_lines) {
            auto split_string = Utility::line_splitter(test_case_line, *test_case_separator);
            const auto created_tc = new Check_runner::TA::Test_case(
                split_string[0],
                split_string[2],
                Check_runner::TA::priority_to_object(split_string[1])
            );
            tmp_test_cases->push_back(*created_tc);
        }
        return *tmp_test_cases;
    }
    Utility::colored_txt_output("Input vector with strings might not be empty");
    throw;
}

/**
 * Create bug object with given parameters.
 * @param bug_name name of the bug to create
 * @param description description of the bug
 * @return constructed bug object.
 */
Check_runner::TA::Bug *Test_artifact_fabric::create_bug(const std::string &bug_name, const std::string &description) const {
    const auto bug = new Check_runner::TA::Bug{bug_name, description};
    return bug;
}

/**
 * Fabric method for check lists
 * @param name checklist name, ex. checklist to make breakfast
 * @param description description of the checklist
 * @param steps steps that need to execute in this checklist
 * @return constructed checklist
 */
Check_runner::TA::Check_list *Test_artifact_fabric::create_check_list(const std::string &name,
                                                        const std::string &description,
                                                        const std::vector<std::string> &steps) const {
    const auto check_list = new Check_runner::TA::Check_list{name, description, steps};
    return check_list;
}

/**
 * Decompose test cases into string line with given style.
 * @param style style to decompose test case.
 * @param tc test case to decompose
 * @return decomposed test case (string line with new line symbols)
 */
std::string Test_artifact_fabric::decompose_test_case(const Check_runner::TA::Test_case &tc, const TS_style style = TS_style::TXT) const {
    switch (style) {
        case TS_style::GOOGLE_STYLESHEET:
            return "";
        case TS_style::JIRA:
            return "";
        case TS_style::JSON:
            return "";
        case TS_style::TEST_RAIL:
            return "";
        case TS_style::TXT: [[likely]]
            return "";
        case TS_style::XML:
            return "";
        default:
            throw std::invalid_argument("Invalid style");
    }
}

/**
 * @param test_case_lines lines to create test cases.
 * @param name name of the test suit.
 * @return created test suit with test cases.
 */
Check_runner::TA::Test_suit<Check_runner::TA::Test_case> *Test_artifact_fabric::create_test_suit(const std::vector<std::string> &test_case_lines, const std::string &name) const {
    const auto ts_suit = new Check_runner::TA::Test_suit{
        name,
        create_test_cases(test_case_lines),
    };
    return ts_suit;
}
