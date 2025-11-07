#include <stdexcept>

#include "Test_Artifact_Fabric.hpp"

Test_artifact_fabric::Test_artifact_fabric() = default;

Test_artifact_fabric::~Test_artifact_fabric() = default;

/**
 * Create just one test case with given parameters.
 * @param tc_name name of the test case.
 * @param description description of the test case.
 * @param priority priority of the test case to create.
 * @return constructed test case with parameters
 */
TA::Test_case Test_artifact_fabric::create_test_case(const std::string &tc_name, const std::string &description, const TA::Priority &priority) const {
    return TA::Test_case(tc_name, description, priority);
}

/**
 * Create test cases with use of given vector strings to create.
 * @param string_lines string lines to create test cases
 * @return vector with test cases, instead of strings
 */
std::vector<TA::Test_case> &Test_artifact_fabric::create_test_cases(const std::vector<std::string> &string_lines) const {
    if (!string_lines.empty()) {
        const auto tmp_test_cases = new std::vector<TA::Test_case>(string_lines.size());
        for (const auto &test_case_line: string_lines) {
            auto split_string = utility::line_splitter(test_case_line, *test_case_separator);
            auto created_tc = this->create_test_case(
                split_string.at(0), //name
                split_string.at(1), //description
                TA::priority_to_object(split_string.at(2)) //priority of the test case
            );
            tmp_test_cases->push_back(created_tc);
        }
        return *tmp_test_cases;
    }
    utility::colored_txt_output("Input vector with strings might not be empty");
    throw;
}

/**
 * Delete test case by its name from vector.
 * @param test_cases vector with test cases.
 * @param name name of the test case in suit to delete.
 */
void Test_artifact_fabric::delete_test_case(const std::vector<TA::Test_case> &test_cases, const std::string &name) const {
    //
}

/**
 * Create bug object with given parameters.
 * @param bug_name name of the bug to create
 * @param description description of the bug
 * @return constructed bug object.
 */
TA::Bug Test_artifact_fabric::create_bug(const std::string &bug_name, const std::string &description) const {
    return TA::Bug(bug_name, description);
}

/**
 * Fabric method for check lists
 * @param name checklist name, ex. checklist to make breakfast
 * @param description description of the checklist
 * @param steps steps that need to execute in this checklist
 * @return constructed checklist
 */
TA::Check_list Test_artifact_fabric::create_check_list(const std::string &name,
                                                       const std::string &description,
                                                       const std::vector<std::string> &steps) const {
    return TA::Check_list(name, description, steps);
}

/**
 * Decompose test cases into string line with given style.
 * @param style style to decompose test case.
 * @return decomposed test case (string line)
 */
std::string Test_artifact_fabric::decompose_test_case(const TS_style style) const {
    switch (style) {
        default:
            throw std::invalid_argument("Invalid style");
    }
}

/**
 * @param test_case_lines lines to create test cases.
 * @param name name of the test suit.
 * @return created test suit with test cases.
 */
TA::Test_suit<TA::Test_case> Test_artifact_fabric::create_test_suit(const std::vector<std::string> &test_case_lines, const std::string &name) const {
    return TA::Test_suit{
        name,
        create_test_cases(test_case_lines),
    };
}
