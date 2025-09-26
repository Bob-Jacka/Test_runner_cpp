#include "TestCaseFabric.hpp"

#include <stdexcept>

TestCaseFabric::TestCaseFabric() {
}

TestCaseFabric::~TestCaseFabric() {
}

TA::Test_case TestCaseFabric::create_test_case(const std::string &tc_name, const std::string &description, const TA::Priority &priority) const {
    return TA::Test_case{
        .name = tc_name,
        .description = description,
        .priority = priority,
    };
}

std::vector<TA::Test_case> TestCaseFabric::create_test_cases(const std::vector<std::string> &test_cases) const {
    if (!test_cases.empty()) {
        const auto tmp_test_cases = new std::vector<TA::Test_case>(test_cases.size());
        for (const auto &test_case_line: test_cases) {
            auto split_string = utility::line_splitter(test_case_line, *test_case_separator);
            auto created_tc = this->create_test_case(
                split_string.at(0), //name
                split_string.at(1), //description
                split_string.at(2) //priority
            );
            tmp_test_cases->push_back(created_tc);
        }
        return *tmp_test_cases;
    } else {
        utility::colored_txt_output("Input vector might not be empty");
        return {};
    }
}

void TestCaseFabric::delete_test_case() const {
    //
}

TA::Bug TestCaseFabric::create_bug(const std::string &bug_name, const std::string &description) const {
    return TA::Bug{
        .name = bug_name,
        .description = description
    };
}

TA::CheckList TestCaseFabric::create_check_list(const std::string &name,
                                                const std::string &description,
                                                const std::vector<std::string> &steps) const {
    return TA::CheckList{
        .name = name,
        .description = description,
        .steps = steps
    };
}

std::string TestCaseFabric::decompose_test_case(const TS_style style) const {
    switch (style) {
        default:
            throw std::invalid_argument("Invalid style");
    }
}
