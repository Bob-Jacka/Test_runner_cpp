#include <any>
#include <chrono>
#include <filesystem>
#include <memory>
#include <vector>

#include <concepts>

#include "Load_parameters.hpp"
#include "Entities/File_controller.hpp"
#include "Entities/LineParser.hpp"
#include "Exceptions/MainException.hpp"
#include "Entities/TestCaseFabric.hpp"
#include "Exceptions/FileControllerException.hpp"
#include "Strategies/declaration/Random_run_strat.hpp"
#include "Strategies/declaration/High_prior_strat.hpp"
#include "Strategies/declaration/Parallel_strat.hpp"
#include "Strategies/declaration/StratContex.hpp"
#include "Test_artifacts/declaration/Test_result.hpp"

import UtilFuncs_mod;

template<typename T>
using v_string = std::pmr::vector<T>;

auto load_parameters = new LP::Load_parameters();
std::unique_ptr<TestCaseFabric> test_case_fabric;
std::unique_ptr<Strategy::StratContext> context = {};
std::unique_ptr<File_controller> file_reader; ///file controller for actions with filesystem
std::unique_ptr<Line_parser> parser;

int arg_count = 0;

/**
 * Special namespace for functions that used in Main function
 */
namespace Main_utilities {
    consteval auto EXIT_SYM = "exit"; ///symbol for exit in test result input
    consteval auto INPUT_SYM = ">> "; ///symbol for user input

    /**
     * Write test results to file.
     * @param test_results vector with test results.
     * @throw FileControllerException
     */
    void write_to_file(const v_string<TA::Test_result> &test_results) {
        if (auto file_test_results = file_reader->create_test_result_file(); file_test_results.is_open()) {
            for (auto &test_result: test_results) {
                file_test_results << test_result.get_name() << "\n";
                file_test_results << test_result.get_device_name() << "\n";
                file_test_results << test_result.get_description() << "\n";
                file_test_results << "Found Bugs:" << "\n";
                for (auto &bug: test_result.get_bugs()) {
                    file_test_results << "\t" << bug.get_name() << "\n";
                    file_test_results << "\t" << bug.get_description() << "\n";
                    auto test_severity = ""; ///tmp variable for severity
                    switch (bug.get_severity()) {
                        case TA::Severity::Low:
                            test_severity = "Low";
                            break;
                        case TA::Severity::Medium:
                            test_severity = "Medium";
                            break;
                        case TA::Severity::High:
                            test_severity = "High";
                            break;
                        case TA::Severity::Critical:
                            test_severity = "Critical";
                            break;
                        case TA::Severity::Blocker:
                            test_severity = "Blocker";
                            break;
                        default:
                            throw FileControllerException("Error in checking for enum value.");
                    }
                    file_test_results << "\t" << test_severity << "\n";
                }
            }
        } else {
            throw FileControllerException("Error in open file for test results.");
        }
        utility::colored_txt_output("Results written to file");
    }

    /**
    * Get vector with Test results and output information in it.
    * @param vec const reference vector with test results.
    */
    void print_test_results(const v_string<TA::Test_result> &vec) {
        int counter = 1;
        for (auto &elem: vec) {
            utility::print(counter);
            utility::colored_txt_output(elem.get_name());
            utility::colored_txt_output(elem.get_description());
            if (const auto bugs = elem.get_bugs(); !bugs.empty()) {
                for (auto &bug: bugs) {
                    utility::colored_txt_output("\tBug name: " + bug.get_name());
                    utility::colored_txt_output("\tBug description: " + bug.get_description());
                    utility::colored_txt_output("\tBug severity: " + reinterpret_cast<auto>(bug.get_severity()));
                }
            } else {
                utility::colored_txt_output("No bugs in this test case");
            }
            counter++;
        }
    }

    /**
    * Get cli arguments.
    * @param argv string value to scan
    * @return vector with generic objects.
    */
    template<typename T>
    v_string<T> resolve_cli_args(char *argv) {
        if (argv != nullptr) {
            auto vec = v_string();
            for (int i = 1; i < arg_count; ++i) {
                vec.emplace_back(argv);
            }
            return vec;
        }
        return v_string<T>();
    }

    /**
     * Function for printing help to user.
     */
    void print_help() {
        utility::println("Test runner C++ edition");
        utility::println("Utility description: - utility is using for test cycles");
        utility::println("Utility parameters:");
        utility::println("\t1. strategy - used for specifying run strategy");
        utility::println("\t\t1.1 high_prior - for only high priority test run");
        utility::println("\t\t1.2 parallel - for parallel random adding test");
        utility::println("\t\t1.3 random - for pseudo-random strategy mode");

        utility::println("\t2. devices - (optional) for providing devices names to test cycle");
        utility::println("\t3. time_record - for record time, during test case execution");
        utility::println("\t4. colored - for color text output");
        utility::println("\t5. comments - for test case comment output");
        utility::println();
        utility::println();
        utility::println();
    }

    /**
    Function for checking command line arguments.
    @param cont_to_check constant string to check for flag value.
    @throw unknown_compiler_flag if undefined compiler flag occurred.
    */
    void check_flags(const v_string<std::string> &cont_to_check) {
        const auto check_func_full = [&](const std::string &str) -> bool {
            return str.contains("=") and str.contains("--");
        };
        //lambda for checking if string is a flag or not.

        if (cont_to_check.size() == 1 and cont_to_check[0] == "help") {
            print_help();
        } else {
            //if not help
            for (auto &elem: cont_to_check) {
                if (check_func_full(elem)) {
                    auto split_line = utility::line_splitter(elem, '=');
                    const auto flag_name = utility::replace_string_all(split_line[0], "--", "");
                    const std::any flag_value = split_line[1]; //because zero index is flag name

                    //entry point
                    if (flag_name == LP::Static_load_parameters_names::entry) {
                        load_parameters->set_entry_point(reinterpret_cast<const std::string &>(flag_value));
                        break;
                    }
                    //strategies for utility execution
                    if (flag_name == LP::Static_load_parameters_names::strat) {
                        if (reinterpret_cast<const std::string &>(flag_value) == LP::Static_load_parameters_names::high_prior_strat) {
                            utility::colored_txt_output("Using high priority strategy.");
                            context->set_strategy(
                                std::make_unique<auto>(Strategy::High_prior_strat())
                            );
                        } else if (reinterpret_cast<const std::string &>(flag_value) == LP::Static_load_parameters_names::random_strat) {
                            utility::colored_txt_output("Using pseudo run strategy.");
                            context->set_strategy(
                                std::make_unique<auto>(Strategy::Random_run_strat())
                            );
                        } else if (reinterpret_cast<const std::string &>(flag_value) == LP::Static_load_parameters_names::parallel_strat) {
                            utility::colored_txt_output("Using parallel strategy.");
                            context->set_strategy(
                                std::make_unique<auto>(Strategy::Parallel_strat())
                            );
                        } else {
                            throw MainException("No strategy selected.");
                        }
                    }
                    //need for more devices than one
                    if (flag_name == LP::Static_load_parameters_names::devices) {
                        load_parameters->set_devices_entry_point(reinterpret_cast<const std::string &>(flag_value));
                    }
                    //time check flag
                    if (flag_name == LP::Static_load_parameters_names::time_check) {
                        load_parameters->set_is_time_record(reinterpret_cast<const bool &>(flag_value));
                    }
                    //colored flag
                    if (flag_name == LP::Static_load_parameters_names::colored) {
                        load_parameters->set_is_colored(reinterpret_cast<const bool &>(flag_value));
                    }
                    //comments flag
                    if (flag_name == LP::Static_load_parameters_names::comments) {
                        load_parameters->set_is_comments(reinterpret_cast<const bool &>(flag_value));
                    }
                } else {
                    throw MainException("Unknown compiler flag selected"); //kill utility if unknown flag
                }
            }
        }
    }

    /**
     Function for transferring char array into string array.
     @param char_array some char array to proceed
     @param length len of the array
     @return vector with strings
     */
    v_string<std::string> *get_str_array(const char *char_array, const int length) {
        const auto str_array = new v_string(length);
        for (int i = 0; i < length; ++i) {
            str_array[i].emplace_back(&char_array[i]);
        }
        return str_array;
    }

    /**
    * Main cycle of the utility execution - proceed test cases.
    * @param vts vector for test cases.
    * @param vtr vector for test results.
    * @param device optional device name.
    */
    template<typename A, typename B>
        requires std::derived_from<TA::Test_case, A> and std::derived_from<TA::Test_result, B>
    void main_cycle(const A &vts, B &vtr, const std::string &device = "Single_device_mode") {
        std::chrono::steady_clock::time_point start;
        std::chrono::steady_clock::time_point end;

        for (const auto &ts: vts) {
            //proceed test case one by one
            utility::colored_txt_output("Name: " + utility::trim(ts.get_name()));
            if (load_parameters->get_is_comments()) {
                utility::colored_txt_output("Comment: " + ts.get_comment()); //output comments to console
            }
            //get start time of the test case execution
            if (load_parameters->get_is_time_record()) {
                start = std::chrono::steady_clock::now();
            }

            //ask user about result block
            std::string action; {
                utility::colored_txt_output("Is test case successful?");
                utility::colored_txt_output("Enter yes (y), no (n) or skip for test result");

                utility::print(INPUT_SYM);
                utility::userInput(action);
                utility::println(); //new line
            }

            while (true) {
                //test case result user input
                auto result = TA::Test_result();
                if (action == "yes" or action == "y" or action == "skip") {
                    result.set_name(ts.get_name());
                    result.set_result(action);
                } else if (action == "no" or action == "n") {
                    result.set_name(ts.get_name());
                    result.set_result(action);

                    utility::println("Write down bugs parameters:");
                    utility::println("To exit enter 'exit' word");

                    std::string bug_name;
                    std::string bug_description;
                    std::string bug_severity;

                    while (true) {
                        utility::print("Enter bug name: ");
                        bug_name = utility::userInput<std::string>();
                        if (bug_name == EXIT_SYM) {
                            break;
                        }
                        utility::println();

                        utility::print("Enter bug description (shortly): ");
                        bug_description = utility::userInput<std::string>();
                        if (bug_description == EXIT_SYM) {
                            break;
                        }
                        utility::println();

                        utility::print("Enter bug severity: ");
                        bug_severity = utility::userInput<std::string>();
                        if (bug_severity == EXIT_SYM) {
                            break;
                        }
                        utility::println();

                        TA::Severity severity;
                        switch (bug_severity) {
                            case "Low":
                                severity = TA::Severity::Low;
                                [[fallthrough]]
                            case "Medium":
                                severity = TA::Severity::Medium;
                                [[fallthrough]]
                            case "High":
                                severity = TA::Severity::High;
                                [[fallthrough]]
                            case "Blocker":
                                severity = TA::Severity::Blocker;
                                [[fallthrough]]
                            default:
                                throw;
                        }

                        const auto bug = TA::Bug(bug_name, bug_description, severity);
                        result.add_bug(bug);
                        result.set_device_name(device); //set device name, if not provided - write single_device_mode constant
                        break;
                    }
                    vtr.push_back(result);
                    break;
                } else {
                    //it can be no exception, if you want
                    throw MainException("Unknown test result.");
                }
            }
            if (load_parameters->get_is_time_record()) {
                end = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsed_seconds = end - start;
                utility::colored_txt_output("Seconds for this test case: " + std::to_string(elapsed_seconds.count()));
            }
        }
    }
}

void main(const int argc, char *args) {
    arg_count = argc;
    Main_utilities::check_flags(Main_utilities::resolve_cli_args<auto>(args)); //proceed flags to load_parameters structure
    file_reader = std::make_unique<File_controller>(load_parameters->get_entry_point());
    parser = std::make_unique<Line_parser>();

    //return modified vector with ts, after all transformations:
    //1) get data from file,
    //2) delete comments from file,
    //3) create test cases
    v_string vts = context->get_strat()->doAlgorithm(test_case_fabric->create_test_cases(parser->parse_lines_empty(file_reader->readlines())));
    v_string vtr = std::vector<TA::Test_result>(); //for test results after test case run

    //Execute main cycle of the utility
    if (!load_parameters->get_devices_entry_point().empty()) {
        for (v_string devices = File_controller::readlines(); const auto &dev: devices) {
            //get devices from file
            Main_utilities::main_cycle(vts, vtr, dev);
        }
    } else {
        Main_utilities::main_cycle(vts, vtr);
    }

    //proceed vector with test results at the end of cycle
    Main_utilities::print_test_results(vtr);

    //write test results to file if parameter
    if (load_parameters->get_is_file_write()) {
        Main_utilities::write_to_file(vtr);
    }
    utility::colored_txt_output("Out utility, bye");
}
