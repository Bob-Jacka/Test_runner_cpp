//Uncomment "TEST" macros to turn on test functionality

#define TEST

#ifdef TEST
#include "Test_dependencies/catch_amalgamated.hpp"

#include "../core/Entities/Ini/Ini_parser.hpp"
#include "../core/Entities/File_controller.hpp"
#include "../core/Entities/Test_Artifact_Fabric.hpp"
#include "../core/Entities/Line/LineParser.hpp"
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
                const auto sut = new Bug("Test bug", "Test description", Severity::Blocker);
                SECTION("Check for parameters") {
                    REQUIRE(sut->get_name() == "Test bug");
                    REQUIRE(sut->get_description() == "Test description");
                    REQUIRE(sut->get_severity() == Check_runner::TA::Severity::Blocker);
                }
                delete sut;
            }

            TEST_CASE("Should create bug entity with empty parameters", positive) {
                const auto sut = new Bug("", "", Severity::Low);
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
#endif
    }

    /**
     * Namespace for ini parser.
     * Separate from Entities test.
     */
    namespace Ini_parser_test {
        using namespace Interpreter_ns;
        constexpr auto file_name = "";

        TEST_CASE("Should construct object from file", positive) {
            const auto sut = new Ini_parser(file_name);
            SECTION("Check for parameters") {
                REQUIRE(not sut->get_sections().empty());
                REQUIRE(sut->get_section_count() != 0);
            }
            delete sut;
        }

        TEST_CASE("Should construct object from given vector object", positive) {
            const auto sut = new Ini_parser();
            SECTION("Check for parameters") {
                REQUIRE(not sut->get_sections().empty());
                REQUIRE(sut->get_section_count() != 0);
            }
            delete sut;
        }

        TEST_CASE("Should not parse ini file, because no file found", negative) {
            REQUIRE_THROWS(new Ini_parser("fake_file_name.ini"));
        }

        TEST_CASE("Should get string value from ini file", positive) {
            const auto sut = new Ini_parser(file_name);
            SECTION("Check for parameters") {
                REQUIRE(not sut->get_sections().empty());
                REQUIRE(sut->get_section_count() != 0);

                REQUIRE(sut->get_value<std::string>("StringSection.test1") == "test_value_string1");
                REQUIRE(sut->get_value<std::string>("StringSection.test2") == "test_value_string2");
                REQUIRE(sut->get_value<std::string>("StringSection.test3") == "test_value_string3");
            }
            delete sut;
        }

        TEST_CASE("Should get bool value from ini file", positive) {
            const auto sut = new Ini_parser(file_name);
            SECTION("Check for parameters") {
                REQUIRE(not sut->get_sections().empty());
                REQUIRE(sut->get_section_count() != 0);

                REQUIRE(sut->get_value<bool>("BoolSection.test1") == true);
                REQUIRE(sut->get_value<bool>("BoolSection.test2") == true);
                REQUIRE(sut->get_value<bool>("BoolSection.test3") == false);
            }
            delete sut;
        }

        TEST_CASE("Should get int value from ini file", positive) {
            const auto sut = new Ini_parser(file_name);
            SECTION("Check for parameters") {
                REQUIRE(not sut->get_sections().empty());
                REQUIRE(sut->get_section_count() != 0);

                REQUIRE(sut->get_value<int>("IntSection.test1") == 1);
                REQUIRE(sut->get_value<int>("IntSection.test2") == 2);
                REQUIRE(sut->get_value<int>("IntSection.test3") == 3);
            }
            delete sut;
        }

        TEST_CASE("Should throw error because no value in ini file", positive) {
            const auto sut = new Ini_parser(file_name);
            SECTION("Check for parameters") {
                REQUIRE(not sut->get_sections().empty());
                REQUIRE(sut->get_section_count() != 0);

                REQUIRE_THROWS(sut->get_value<int>("IntSection.falseVal1") == 1);
                REQUIRE_THROWS(sut->get_value<bool>("IntSection.falseVal2") == false);
                REQUIRE_THROWS(sut->get_value<std::string>("IntSection.falseVal3") == "false_val");
            }
            delete sut;
        }

        TEST_CASE("Should get value from ini file with default val", positive) {
            const auto sut = new Ini_parser(file_name);
            SECTION("Check for parameters") {
                REQUIRE(not sut->get_sections().empty());
                REQUIRE(sut->get_section_count() != 0);

                REQUIRE(sut->get_value_or<int>("IntSection.test1", 0) == 1);
                REQUIRE(sut->get_value_or<bool>("BoolSection.test1", true) == false);
                REQUIRE(sut->get_value_or<std::string>("StringSection.test1", "") == "test_value_string3");
            }
            delete sut;
        }

        TEST_CASE("Should get default value from ini file because no value found and no throw", positive) {
            const auto sut = new Ini_parser(file_name);
            SECTION("Check for parameters") {
                REQUIRE(not sut->get_sections().empty());
                REQUIRE(sut->get_section_count() != 0);

                REQUIRE_NOTHROW(sut->get_value_or<int>("FakeSect.test1", 0) == 0);
                REQUIRE_NOTHROW(sut->get_value_or<bool>("FakeSect.test2", true) == true);
                REQUIRE_NOTHROW(sut->get_value_or<std::string>("FakeSect.test3", "fake") == "fake");
            }
            delete sut;
        }
    }

    /**
     * Namespace for File controller and Test artifact fabric tests
     */
    namespace Entities_test {
        namespace File_controller_test {
            constexpr auto file_name = "test_file.txt";

            TEST_CASE("String create file controller", positive) {
                REQUIRE_NOTHROW(new File_controller());
            }

            TEST_CASE("Should read all lines from file", positive) {
                const auto sut = new File_controller();
                SECTION("Check for parameters") {
                    REQUIRE(not sut->readlines(file_name).empty());
                }
                delete sut;
            }

            TEST_CASE("Should read only one line from file", positive) {
                const auto sut = new File_controller();
                SECTION("Check for parameters") {
                    const auto line = sut->readline(file_name);

                    REQUIRE(not line.empty());
                    REQUIRE(line.size() == 1);
                }
                delete sut;
            }
        }

        namespace Test_case_fabric_test {
            TEST_CASE("String create test artifact fabric", positive) {
                REQUIRE_NOTHROW(new Check_runner::TA::Test_artifact_fabric());
            }
        }

        namespace Line_parser_test {
            TEST_CASE("String create line parser", positive) {
                REQUIRE_NOTHROW(new Interpreter_ns::DirectiveInterpreter());
            }
        }
    }
}

/**
 * suits for run only groups of tests
 */
namespace Test_utility_suit {
    TEST_CASE("Run only positive tests") {
        //
    }

    TEST_CASE("Run only negative tests") {
        //
    }
}

#endif
