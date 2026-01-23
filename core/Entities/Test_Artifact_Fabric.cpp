#include <stdexcept>

#include "Test_Artifact_Fabric.hpp"

Check_runner::TA::Test_artifact_fabric::Test_artifact_fabric() = default;

Check_runner::TA::Test_artifact_fabric::~Test_artifact_fabric() = default;

/**
 * Create test cases with use of given vector strings to create.
 * @param string_lines string lines to create test cases
 * @throw std::runtime_error in case of empty input vector
 * @return vector with test cases, instead of strings
 */
std::vector<Check_runner::TA::Test_case> &Check_runner::TA::Test_artifact_fabric::create_test_cases(
    const std::vector<std::string> &string_lines) const {
    if (!string_lines.empty()) {
        const auto tmp_test_cases = new std::vector<Test_case>();
        for (auto &test_case_line: string_lines) {
            auto split_string = Utility::split(test_case_line, *test_case_separator_sym);
            if (split_string.size() == 2) { //in case of empty comment, just add empty string
                split_string.emplace_back("");
            }
            const auto created_tc = new Test_case(
                split_string[0],
                split_string[2],
                priority_to_object(split_string[1])
            );
            tmp_test_cases->push_back(*created_tc);
        }
        return *tmp_test_cases;
    }
    throw std::runtime_error("Input vector with strings might not be empty");
}

/**
 * Create bug object with given parameters.
 * @param bug_name name of the bug to create
 * @param description description of the bug
 * @return constructed bug object.
 */
Check_runner::TA::Bug *Check_runner::TA::Test_artifact_fabric::create_bug(const std::string &bug_name, const std::string &description) const {
    const auto bug = new Bug{bug_name, description};
    return bug;
}

/**
 * Fabric method for check lists
 * @param name checklist name, ex. checklist to make breakfast
 * @param description description of the checklist
 * @param steps steps that need to execute in this checklist
 * @return constructed checklist
 */
Check_runner::TA::Check_list *Check_runner::TA::Test_artifact_fabric::create_check_list(const std::string &name,
                                                                                        const std::string &description,
                                                                                        const std::vector<std::string> &steps) const {
    const auto check_list = new Check_list{name, description, steps};
    return check_list;
}

/**
 * Decompose test cases into string line with given style.
 * @param style style to decompose test case.
 * @param tc test case to decompose.
 * @throw std::invalid_argument if wrong style or style not supported by method.
 * @return decomposed test case (string line with new line symbols) to write into file or another or return "" if not implemented.
 */
std::string Check_runner::TA::Test_artifact_fabric::decompose_test_case(const Test_case &tc, const TS_style style = TS_style::TXT) const {
    auto add_line = [&](const std::string &line) -> std::string {
        return line + "\n";
    };
    std::string decomposed_test_case;
    switch (style) {
            [[unlikely]] case TS_style::GOOGLE_STYLESHEET:
            return "";

            [[unlikely]] case TS_style::JIRA:
            return "";

            [[unlikely]] case TS_style::JSON:
            return "";

            [[unlikely]] case TS_style::TEST_RAIL:
            return "";

            [[likely]] case TS_style::TXT:
            decomposed_test_case += add_line(tc.get_name());
            decomposed_test_case += add_line(priority_to_object(tc.get_priority()));
            decomposed_test_case += add_line(tc.get_comment());
            decomposed_test_case += add_line(""); //simply add new line
            return std::move(decomposed_test_case);

            [[unlikely]] case TS_style::XML:
            return "";
        default:
            throw std::invalid_argument("Invalid style");
    }
}

#ifdef EXTENDED_FUNCTIONALITY
/**
 * @param test_case_lines lines to create test cases.
 * @param name name of the test suit.
 * @return created test suit with test cases.
 */
Check_runner::TA::Test_suit<Check_runner::TA::Test_case> *Check_runner::TA::Test_artifact_fabric::create_test_suit(
    const std::vector<std::string> &test_case_lines, const std::string &name) const {
    const auto ts_suit = new Test_suit<Test_case>{
        name,
        create_test_cases(test_case_lines),
    };
    return ts_suit;
}
#endif
