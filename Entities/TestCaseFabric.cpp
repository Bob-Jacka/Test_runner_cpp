#include "TestCaseFabric.hpp"


TestCaseFabric::TestCaseFabric() {
}

TestCaseFabric::~TestCaseFabric() {
}

TA::Test_case TestCaseFabric::create_test_case() const {
    return TA::Test_case{
        .name = "create_test_case",
        .description = decompose_test_case(TS_style::TXT)
    };
}

TA::Test_case TestCaseFabric::delete_test_case() const {
    //
}

TA::Bug TestCaseFabric::create_bug() const {
    return TA::Bug{
        .name = "create_bug",
        .description = decompose_test_case(TS_style::TXT)
    };
}

TA::CheckList TestCaseFabric::create_check_list() const {
    //
}

std::string TestCaseFabric::decompose_test_case(TS_style style) const {
    return;
}

void TestCaseFabric::delete_test_case() const {
    //
}
