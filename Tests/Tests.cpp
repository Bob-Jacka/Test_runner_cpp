//Uncomment "TEST" macros to turn on test functionality

#define TEST

#ifdef TEST
#include "Test_dependencies/catch_amalgamated.hpp"

#include "../core/Test_artifacts/declaration/Bug.hpp"
#include "../core/Test_artifacts/declaration/Check_list.hpp"
#include "../core/Test_artifacts/declaration/Test_case.hpp"
#include "../core/Test_artifacts/declaration/Test_suit.hpp"
#include "../core/Test_artifacts/declaration/Test_result.hpp"
import UtilFuncs_mod;

/**
 * One namespace to rule the World (for all another namespaces with tests).
 */
namespace Test {
    constexpr auto positive = "Positive";
    constexpr auto negative = "Negative";

    /**
     * Namespace for utilities file
     */
    namespace UtilFuncs {
        using namespace Utility;
        TEST_CASE("String should contain another string", positive) {
            REQUIRE(contains("test_string", "string"));
        }

        TEST_CASE("String should not contain another string", negative) {
            REQUIRE(contains("test", "string"));
        }

        /**
         * Pack of atob function tests
         */
        namespace Atob {
            TEST_CASE("String should cast string into boolean1", positive) {
                REQUIRE(atob("False"));
            }

            TEST_CASE("String should cast string into boolean2", positive) {
                REQUIRE(atob("false"));
            }

            TEST_CASE("String should cast string into boolean3", positive) {
                REQUIRE(atob("True"));
            }

            TEST_CASE("String should cast string into boolean4", positive) {
                REQUIRE(atob("true"));
            }

            TEST_CASE("String should not cast string into boolean5", negative) {
                REQUIRE(atob("condition"));
            }
        }

        /**
         * Namespace for replace functions in utility namespace
         */
        namespace Replace {
            TEST_CASE("Should replace character in string with another character", positive) {
                REQUIRE(replace(std::string("tes1"), '1', 't'));
            }

            TEST_CASE("Should replace one string with another string in source string") {
                REQUIRE(replace(std::string("Bye world"), "Bye", "Hello"));
            }

            TEST_CASE() {
                REQUIRE(replace_string_all());
            }
        }

        TEST_CASE("Should split string") {
            REQUIRE(line_splitter());
        }

        namespace Trim {
            TEST_CASE("Should trim string") {
                REQUIRE(trim());
            }

            TEST_CASE() {
                REQUIRE(trim_start());
            }

            TEST_CASE() {
                REQUIRE(trim_end());
            }
        }
    }

    /**
     * Namespace for all test artifacts in utility
     */
    namespace Artifacts {
        using namespace Check_runner::TA;

        namespace Bugs_test {
            TEST_CASE("Should create bug entity with all valid parameters", positive) {
                const auto sut = new Check_runner::TA::Bug("Test bug", "Test description", Check_runner::TA::Severity::Blocker);
                SECTION("Check for parameters") {
                    REQUIRE(sut->get_name() == "Test bug");
                    REQUIRE(sut->get_description() == "Test description");
                    REQUIRE(sut->get_severity() == Check_runner::TA::Severity::Blocker);
                }
                delete sut;
            }

            TEST_CASE("Should create bug entity with empty parameters", positive) {
                const auto sut = new Check_runner::TA::Bug("", "", Check_runner::TA::Severity::Low);
                SECTION("Check for parameters") {
                    REQUIRE(sut->get_name() == "");
                    REQUIRE(sut->get_description() == "");
                    REQUIRE(sut->get_severity() == Check_runner::TA::Severity::Low);
                }
                delete sut;
            }
        }


#ifdef EXPERIMENTAL
        namespace CheckList_test {
            TEST_CASE("Should create check list without steps", positive) {
                const auto sut = new Check_list("Check list name", "Check list description");
                SECTION("Check for parameters") {
                    REQUIRE(sut->get_name() == "Check list name");
                    REQUIRE(sut->get_description() == "Check list description");
                    REQUIRE(sut->get_steps().size() == 0);
                }
                delete sut;
            }

            TEST_CASE("Should create check list with steps", positive) {
                const auto sut = new Check_list();
                SECTION("Check for parameters") {
                    REQUIRE();
                    REQUIRE();
                    REQUIRE();
                }
                delete sut;
            }
        }

        namespace Test_case_test {
            TEST_CASE("Should create test case with valid parameters", positive) {
                const auto sut = new Test_case("Test name", "Test description", Priority::Critical, Severity::Low);
                SECTION("Check for parameters") {
                    REQUIRE();
                    REQUIRE();
                    REQUIRE();
                }
                delete sut;
            }
        }

        namespace Test_result_test {
            TEST_CASE("Should create test result with valid parameters", positive) {
                const auto sut = new Test_result();
                SECTION("Check for parameters") {
                    REQUIRE();
                    REQUIRE();
                    REQUIRE();
                }
                delete sut;
            }
        }

        namespace Test_suit_test {
            TEST_CASE("Should create test suit with only one parameter", positive) {
                auto sut = new Test_suit<Test_case>("Reconnect suit");
                SECTION("Check for parameters") {
                    REQUIRE();
                    REQUIRE();
                    REQUIRE();
                }
                delete sut;
            }

            TEST_CASE("Should create test suit with two parameters", positive) {
                auto sut = new Test_suit<Test_case>("Reconnect suit", new std::vector<Test_case>());
                SECTION("Check for parameters") {
                    REQUIRE();
                    REQUIRE();
                    REQUIRE();
                }
                delete sut;
            }
        }

        namespace Entities {
            TEST_CASE() {
                REQUIRE();
            }
        }
#endif
    }
}

#endif
