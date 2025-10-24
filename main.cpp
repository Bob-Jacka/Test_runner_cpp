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
using Vec = std::pmr::vector<T>; ///short type for vector

/**
 * Structure for entities data in this utility.
 */
struct Utility_entities {
    static std::unique_ptr<LP::Load_parameters> load_parameters; ///load parameters of the utility
    static std::unique_ptr<TestCaseFabric> test_case_fabric; ///test artifacts fabric entity
    static std::unique_ptr<Strategy::StratContext> context; ///context for determining utility strategy
    static std::unique_ptr<File_controller> file_reader; ///file controller for actions with filesystem
    static std::unique_ptr<Line_interpreter_ns::DirectiveInterpreter> parser; ///entity for text parsing in suit
};

int arg_count = 0;

/**
 * Special namespace for functions that used in Main function
 */
namespace Main_utilities {
    consteval auto EXIT_SYM = "exit"; ///symbol for exit in test result input
    consteval auto INPUT_SYM = ">> "; ///symbol for user input

    /**
     * Function for checking if file exists in filesystem.
     * @param file_name name of the file to check.
     */
    bool check_file_existence(const std::string &file_name) {
        return std::filesystem::exists(file_name);
    }

    /**
     * Check for file extension.
     * @param file_name name of the file to check.
     * @return bool value of checking.
     */
    bool check_file_extension(const std::string &file_name) {
        if (file_name.ends_with(".txt") or file_name.contains(".")) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * Write test results to file.
     * @param test_results vector with test results.
     * @throw FileControllerException
     */
    void write_test_results_2file(const Vec<TA::Test_result> &test_results) -> void {
        if (auto file_test_results = Utility_entities::file_reader->create_test_result_file(); file_test_results.is_open()) {
            for (auto &test_result: test_results) {
                file_test_results << test_result.get_name() << "\n";
                file_test_results << test_result.get_device_name() << "\n";
                file_test_results << test_result.get_description() << "\n";
                if (const auto bugs = test_result.get_bugs(); !bugs.empty()) {
                    int counter = 1; //if you are a programmer - you can start from 0 value!
                    file_test_results << "Found Bugs:" << "\n";
                    for (auto &bug: bugs) {
                        file_test_results << counter << ") " << "\n";
                        file_test_results << "\tBug name: " << bug.get_name() << "\n";
                        file_test_results << "\tBug description: " << bug.get_description() << "\n";
                        auto test_severity = ""; ///tmp variable for severity string value
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
                        file_test_results << "\tBug severity: " << test_severity << "\n";
                        counter++;
                    }
                } else {
                    //no bug found branch
                    file_test_results << "No bugs found" << "\n";
                }
            }
        } else {
            throw FileControllerException("Error in open file for test results.");
        }
        utility::colored_txt_output("Results written to test results file");
    }

    /**
    * Get vector with Test results and output information in it.
    * @param vec const reference vector with test results.
    */
    void print_test_results_2console(const Vec<TA::Test_result> &vec) {
        int counter = 1;
        for (auto &elem: vec) {
            //elem = Test result object
            utility::print(counter);
            utility::colored_txt_output(elem.get_name());
            utility::colored_txt_output(elem.get_description());
            if (const auto bugs = elem.get_bugs(); !bugs.empty()) {
                for (const auto &bug: bugs) {
                    utility::colored_txt_output("\tBug name: " + bug.get_name());
                    utility::colored_txt_output("\tBug description: " + bug.get_description());
                    utility::colored_txt_output("\tBug severity: " + reinterpret_cast<auto>(bug.get_severity()));
                }
            } else {
                utility::colored_txt_output("No bugs in this test case");
            }
            ++counter;
        }
    }

    /**
    * Get cli arguments and add them to vector entity.
    * @param argv string value to scan
    * @return vector with generic objects.
    */
    template<typename T>
    Vec<T> resolve_cli_args(const char *argv) {
        if (argv != nullptr) {
            auto vec = Vec(arg_count);
            auto split_line = std::vector<std::string>(arg_count);
            utility::split(argv, split_line, ' ');
            for (int i = 1; i < arg_count; ++i) {
                vec.emplace_back(split_line[i]);
            }
            return vec;
        }
        return Vec<T>();
    }

    /**
     * Function for printing help to user.
     */
    void print_help() -> void {
        utility::println("Test runner C++ edition");
        utility::println("Utility description: - utility is using for test cycles");
        utility::println(); //just new line in console
        utility::println("Utility parameters:");
        utility::println("\t1. 'strategy' - used for specifying run strategy");
        utility::println("\t\t1.1 'high_prior' - for only high priority test run");
        utility::println("\t\t1.2 'parallel' - for parallel random adding test");
        utility::println("\t\t1.3 'random' - for pseudo-random strategy mode");
        utility::println("\t2. 'devices' - (optional) for providing devices names to test cycle");
        utility::println("\t3. 'time_record' - for record time, during test case execution");
        utility::println("\t4. 'colored' - for color text output");
        utility::println("\t5. 'comments' - for test case comment output");
        utility::println(); //just new line in console
        utility::println("Available utility directives in files with tests:");
        utility::println("\t1. 'suit_start' - directive for indication for test suit start,");
        utility::println("\t2. 'suit_end' - directive for indication for test suit end,");
        utility::println("\t3. 'if' - usual if operator for branching in many programming languages,");
        utility::println("\t4. 'else' - additional directive for if");
        utility::println("\t5. 'end_if' - closes if condition");
    }

    /**
    * Function for checking command line arguments.
    * @param cont_to_check constant string vector to check for flag value.
    * @throw unknown_compiler_flag if undefined compiler flag occurred.
    */
    void check_flags(const Vec<std::string> &cont_to_check) {
        const auto check_func_full = [&](const std::string &str) -> bool {
            return str.contains("=") and str.contains("--");
        }; ///lambda for checking if string is a flag or not.
        using con_string_ref = const std::string &; ///short version of type

        if (cont_to_check.empty()) {
            throw MainException("flags vector is empty");
        } else {
            for (auto &cli_param: cont_to_check) {
                if (check_func_full(cli_param)) {
                    auto split_line = utility::line_splitter(cli_param, '=');
                    const auto flag_name = utility::replace_string_all(split_line[0], "--", "");
                    const std::any flag_value = split_line[1]; //because zero index is flag name

                    //get entry point of utility working
                    if (flag_name == LP::Static_load_parameters_names::entry) {
                        auto entry_point = reinterpret_cast<con_string_ref>(flag_value);
                        //check for file extension
                        if (!check_file_extension(entry_point)) {
                            //TODO может быть проблема в логике
                            //if true - user provided ext //else - user does not provide ext and need to append ext for correct utility work
                            entry_point += ".txt";
                            if (check_file_existence(entry_point)) {
                                Utility_entities::load_parameters->set_entry_point(entry_point);
                            } else {
                                throw MainException("Entry point file does not exists in filesystem");
                            }
                        } else {
                            throw MainException("Wrong file extension in file name: " + entry_point);
                        }
                        break;
                    }
                    //strategies for utility execution
                    if (flag_name == LP::Static_load_parameters_names::strat) {
                        const auto val = reinterpret_cast<con_string_ref>(flag_value);
                        if (val == LP::Static_load_parameters_names::high_prior_strat) {
                            utility::colored_txt_output("Using high priority strategy.");
                            Utility_entities::context->set_strategy(
                                std::make_unique<auto>(Strategy::High_prior_strat())
                            );
                        } else if (val == LP::Static_load_parameters_names::random_strat) {
                            utility::colored_txt_output("Using pseudo run strategy.");
                            Utility_entities::context->set_strategy(
                                std::make_unique<auto>(Strategy::Random_run_strat())
                            );
                        } else if (val == LP::Static_load_parameters_names::parallel_strat) {
                            utility::colored_txt_output("Using parallel strategy.");
                            Utility_entities::context->set_strategy(
                                std::make_unique<auto>(Strategy::Parallel_strat())
                            );
                        } else {
                            throw MainException("No strategy selected.");
                        }
                    }
                    //need for more devices than one
                    if (flag_name == LP::Static_load_parameters_names::devices) {
                        Utility_entities::load_parameters->set_devices_entry_point(reinterpret_cast<con_string_ref>(flag_value));
                    }
                    //time check flag
                    if (flag_name == LP::Static_load_parameters_names::time_check) {
                        Utility_entities::load_parameters->set_is_time_record(reinterpret_cast<const bool &>(flag_value));
                    }
                    //colored flag
                    if (flag_name == LP::Static_load_parameters_names::colored) {
                        Utility_entities::load_parameters->set_is_colored(reinterpret_cast<const bool &>(flag_value));
                    }
                    //comments flag
                    if (flag_name == LP::Static_load_parameters_names::comments) {
                        Utility_entities::load_parameters->set_is_comments(reinterpret_cast<const bool &>(flag_value));
                    }
                } else {
                    throw MainException("Unknown compiler flag selected: " + cli_param); //kill utility if unknown flag detected
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
    Vec<std::string> *convert_from_str_2vector(const char *char_array, const int length) {
        const auto str_array = new Vec(length);
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
    * @throws MainException("Unknown test result."), MainException("Unknown test severity.")
    */
    template<typename A, typename B>
        requires std::derived_from<TA::Test_case, A> and std::derived_from<TA::Test_result, B>
    void main_utility_cycle(const A &vts, B &vtr, const std::string &device = "Single_device_mode") {
        std::chrono::steady_clock::time_point start; //start time of ts execution
        std::chrono::steady_clock::time_point end;

        for (const auto &ts: vts) {
            //proceed test case one by one
            utility::colored_txt_output("Name: " + utility::trim(ts.get_name()));
            if (Utility_entities::load_parameters->get_is_comments()) {
                utility::colored_txt_output("Comment: " + ts.get_comment()); //output comments to console
            }
            //get start time of the test case execution
            if (Utility_entities::load_parameters->get_is_time_record()) {
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

            //result forever loop
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
                        if (bug_name == Main_utilities::EXIT_SYM) {
                            break;
                        }
                        utility::println();

                        utility::print("Enter bug description (shortly, if you can): ");
                        bug_description = utility::userInput<std::string>();
                        if (bug_description == Main_utilities::EXIT_SYM) {
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
                                throw MainException("Unknown test severity."); //you can write endless loop to not terminate program
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
            if (Utility_entities::load_parameters->get_is_time_record()) {
                end = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsed_seconds = end - start;
                utility::colored_txt_output("Seconds for this test case: " + std::to_string(elapsed_seconds.count()));
            }
        }
    }
}

/**
 * Program entry point
 * @param argc
 * @param args
 * @throw MainException if necessary files not found
 */
void main(const int argc, const char *args) {
    arg_count = argc;

    if (arg_count > 1) {
        ///Entity init block
        {
            Utility_entities::load_parameters = std::make_unique<LP::Load_parameters>();
            Main_utilities::check_flags(Main_utilities::resolve_cli_args<auto>(args)); //proceed flags to load_parameters structure
            Utility_entities::file_reader = std::make_unique<File_controller>(Utility_entities::load_parameters->get_entry_point());
            Utility_entities::parser = std::make_unique<Line_interpreter_ns::DirectiveInterpreter>();
        }

        ///Modified vector with ts, after all transformations:
        Utility_entities::parser->set_main_suits(Utility_entities::file_reader->readlines()); //1) Get data from entry point file)
        Utility_entities::parser->parse_lines_empty(); //2) Delete comments from file
        Utility_entities::parser->parse_directives(); //2.5) parse directives in suit file

        //3) Create test cases objects
        Vec vts = Utility_entities::context->get_strat()->doAlgorithm(
            Utility_entities::test_case_fabric->create_test_cases(Utility_entities::parser->get_main_suit())
        );
        auto vtr = Vec<TA::Test_result>(); ///vector for test results after test case run

        //Execute main cycle of the utility
        if (!Utility_entities::load_parameters->get_devices_entry_point().empty()) {
            if (Main_utilities::check_file_existence(devices_static_file_name)) {
                for (const Vec devices = File_controller::readlines(); const auto &device: devices) {
                    //get devices from file
                    utility::colored_txt_output("Device name: " + device.get_name());
                    Main_utilities::main_utility_cycle(vts, vtr, device);
                }
            } else {
                throw MainException("No device entry point file was found.");
            }
        } else {
            utility::colored_txt_output("Device name: Single device mode running");
            Main_utilities::main_utility_cycle(vts, vtr);
        }

        //proceed vector with test results at the end of cycle
        Main_utilities::print_test_results_2console(vtr);

        //write test results to file if parameter
        if (Utility_entities::load_parameters->get_is_file_write()) {
            Main_utilities::write_test_results_2file(vtr);
        }
        utility::colored_txt_output("Out utility, bye");
    } else if (const std::string conv_arg = args; arg_count >= 2 and conv_arg.contains("--help")) {
        //print help to user if user wants help
        Main_utilities::print_help();
        exit(EXIT_SUCCESS);
    } else {
        //anywhere print help to user
        Main_utilities::print_help();
        exit(EXIT_FAILURE);
    }
}
