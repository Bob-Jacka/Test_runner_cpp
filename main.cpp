#include <any>
#include <chrono>
#include <filesystem>
#include <memory>
#include <vector>

#include <concepts>

#include "Load_parameters.hpp"
#include "core/Entities/LineParser.hpp"
#include "core/Entities/File_controller.hpp"
#include "core/Entities/LineParser.hpp"
#include "core/Exceptions/MainException.hpp"
#include "core/Entities/Test_artifact_fabric.hpp"
#include "core/Exceptions/FileControllerException.hpp"
#include "core/Strategies/declaration/Random_run_strat.hpp"
#include "core/Strategies/declaration/High_prior_strat.hpp"
#include "core/Strategies/declaration/Parallel_strat.hpp"
#include "core/Strategies/declaration/StratContex.hpp"
#include "core/Strategies/declaration/Usual_strat.hpp"
#include "core/Test_artifacts/declaration/Test_result.hpp"

import UtilFuncs_mod;

template<typename T>
using Vec_t = std::vector<T>; ///short type for vector

// #define EXPERIMENTAL

int arg_count = 0; //count of arguments in utility

/**
 * Special namespace for functions that used in Main function
 */
namespace Check_runner {
    /**
    * Structure for entities data in this utility.
    * Contains entities for functionality and data for test artifacts.
    */
    struct Entities {
        static std::unique_ptr<LP::Load_parameters> load_parameters; ///load parameters of the utility
        static std::unique_ptr<Test_artifact_fabric> test_case_fabric; ///test artifacts fabric entity
        static std::unique_ptr<Strategy::StratContext> context; ///context for determining utility strategy
        static std::unique_ptr<Line_interpreter_ns::DirectiveInterpreter> parser; ///entity for text parsing in suit

        //Vectors with test artifacts
        static Vec_t<TA::Test_result> vtr; ///vector for test results after test case run
        static Vec_t<TA::Test_case> vts; ///vector for test cases, that created in utility
    };

    consteval auto EXIT_SYM = "exit"; ///symbol for exit in test result input
    consteval auto INPUT_SYM = ">> "; ///symbol for user input

    /**
     * Low level functionality (filesystem actions) in utility
     */
    namespace Low_level {
        /**
        * Write down test results to file.
        * @param test_results vector with test results.
        * @throw FileControllerException if unknown bug severity
        */
        void write_test_results_2file(const Vec_t<TA::Test_result> &test_results) -> void {
            if (auto file_test_results = File_controller::create_test_result_file(); file_test_results.is_open()) {
                for (auto &test_result: test_results) {
                    file_test_results << test_result.get_name() << "\n";
                    file_test_results << test_result.get_device_name() << "\n";
                    file_test_results << test_result.get_result() << "\n";
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
                                    throw Check_exceptions::FileControllerException("Error in checking for enum value.");
                            }
                            file_test_results << "\tBug severity: " << test_severity << "\n";
                            ++counter;
                        }
                    } else {
                        //no bug found branch
                        file_test_results << "No bugs found" << "\n";
                    }
                }
            } else {
                throw Check_exceptions::FileControllerException("Error in open file for test results.");
            }
            Utility::colored_txt_output("Results written to test results file");
        }

        void save_current_progress();

        void load_current_progress();

        /**
         * Function for gracefully exit from utility.
         * @param _Code exit code.
         * @param system_msg optional message for message before exit (due to error in utility work)
         */
        void exit_utility(const int _Code, const std::string &system_msg = "") -> void {
            Entities::vtr.clear();
            Entities::vts.clear();

            if (!system_msg.empty()) {
                Utility::println(system_msg);
            } else {
                Utility::println("Bye");
            }
            std::exit(_Code);
        }
    }

    /**
     * Namespace for different checks in utility
     */
    namespace Check {
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
        * Function for checking command line arguments.
        * @param cont_to_check constant string vector to check for flag value.
        * @throw unknown_compiler_flag if undefined compiler flag occurred.
        */
        void check_flags(const Vec_t<std::string> &cont_to_check) {
            const auto check_func_full = [&](const std::string &str) -> bool {
                return str.contains("=") and str.contains("--");
            }; ///lambda for checking if string is a flag or not.
            using con_string_ref = const std::string &; ///short version of type for string values
            using con_bool_ref = const bool &; ///short version of type for boolean values

            if (cont_to_check.empty()) {
                throw Check_exceptions::MainException("flags vector is empty");
            } else {
                for (auto &cli_param: cont_to_check) {
                    if (check_func_full(cli_param)) {
                        auto split_line = Utility::line_splitter(cli_param, '=');
                        const auto flag_name = Utility::replace_string_all(split_line[0], "--", "");
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
                                    Entities::load_parameters->set_entry_point(entry_point);
                                } else {
                                    throw Check_exceptions::MainException("Entry point file does not exists in filesystem");
                                }
                            } else {
                                throw Check_exceptions::MainException("Wrong file extension in file name: " + entry_point);
                            }
                            break;
                        }
                        //strategies for utility execution
                        if (flag_name == LP::Static_load_parameters_names::strat) {
                            const auto val = reinterpret_cast<con_string_ref>(flag_value);
                            if (val == LP::Static_load_parameters_names::high_prior_strat) {
                                Utility::colored_txt_output("Using 'high priority' strategy.");
                                Entities::context->set_strategy(
                                    std::make_unique<auto>(new Strategy::High_prior_strat())
                                );
                            } else if (val == LP::Static_load_parameters_names::random_strat) {
                                Utility::colored_txt_output("Using 'pseudo run' strategy.");
                                Entities::context->set_strategy(
                                    std::make_unique<auto>(new Strategy::Random_run_strat())
                                );
                            } else if (val == LP::Static_load_parameters_names::parallel_strat) {
                                Utility::colored_txt_output("Using 'parallel' strategy.");
                                Entities::context->set_strategy(
                                    std::make_unique<auto>(new Strategy::Parallel_strat())
                                );
                            } else if (val == LP::Static_load_parameters_names::usual_strat) {
                                Utility::colored_txt_output("Using 'usual' strategy.");
                                Entities::context->set_strategy(
                                    std::make_unique<auto>(new Strategy::Usual_strat())
                                );
                            } else {
                                throw Check_exceptions::MainException("No strategy selected for value: " + val);
                            }
                        }
                        //other utility flags block
                        {
                            //set utility usage parameters
                            if (flag_name == LP::Static_load_parameters_names::parameters) {
                                Entities::load_parameters->set_parameters(flag_name);
                            }

                            //need for more devices than one
                            if (flag_name == LP::Static_load_parameters_names::devices) {
                                Entities::load_parameters->set_devices_entry_point(reinterpret_cast<con_string_ref>(flag_value));
                            }

                            //time check flag
                            if (flag_name == LP::Static_load_parameters_names::time_check) {
                                Entities::load_parameters->set_is_time_record(reinterpret_cast<con_bool_ref>(flag_value));
                            }

                            //colored flag
                            if (flag_name == LP::Static_load_parameters_names::colored) {
                                Entities::load_parameters->set_is_colored(reinterpret_cast<con_bool_ref>(flag_value));
                            }

                            //comments flag
                            if (flag_name == LP::Static_load_parameters_names::comments) {
                                Entities::load_parameters->set_is_comments(reinterpret_cast<con_bool_ref>(flag_value));
                            }
                        }
                    } else {
                        throw Check_exceptions::MainException("Unknown compiler flag selected: " + cli_param); //kill utility if unknown flag detected
                    }
                }
            }
        }
    }

    /**
     * Namespace for print functions
     */
    namespace Print {
        /**
        * Get vector with Test results and output information in it.
        * @param vec const reference vector with test results.
        */
        void print_test_results_2console(const Vec_t<TA::Test_result> &vec) {
            int counter = 1;
            for (const auto &res: vec) {
                //elem = Test result object
                Utility::print(counter);
                Utility::colored_txt_output("Test name: " + res.get_name());
                Utility::colored_txt_output("Test result: " + res.get_result());
                if (const auto &bugs_vec = res.get_bugs(); !bugs_vec.empty()) {
                    for (const auto &bug: bugs_vec) {
                        Utility::colored_txt_output("\tBug name: " + bug.get_name());
                        Utility::colored_txt_output("\tBug description: " + bug.get_description());
                        Utility::colored_txt_output("\tBug severity: " + reinterpret_cast<auto>(bug.get_severity()));
                    }
                } else {
                    Utility::colored_txt_output("No bugs in this test case");
                }
                ++counter;
            }
        }

        /**
        * Function for printing help to user.
        */
        void print_help() -> void {
            Utility::println("Test runner C++ edition");
            Utility::println("Utility description: - utility is using for test cycles\n");
            Utility::println("Utility parameters:");
            Utility::println("\t1. 'strategy' - used for specifying run strategy");
            Utility::println("\t\t1.1 'high_prior' - for only high priority test run");
            Utility::println("\t\t1.2 'parallel' - for parallel random adding test");
            Utility::println("\t\t1.3 'random' - for pseudo-random strategy mode");
            Utility::println("\t2. 'devices' - (optional) for providing devices names to test cycle");
            Utility::println("\t3. 'time_record' - for record time, during test case execution");
            Utility::println("\t4. 'colored' - for color text output");
            Utility::println("\t5. 'comments' - for test case comment output\n");
            Utility::println("Available utility directives in files with tests:");
            Utility::println("\t1. 'Group_start' - directive for indication for test suit start,");
            Utility::println("\t2. 'Group_end' - directive for indication for test suit end,");
            Utility::println("\t3. 'If' - usual if operator for branching in many programming languages,");
            Utility::println("\t4. 'Else' - additional directive for if");
            Utility::println("\t5. 'End_if' - closes if condition");
        }

        /**
         * Function for printing bugs during test run (not for save in file or print in console)
         */
        void see_bugs() {
            for (const auto &elem: Entities::vtr) {
                Utility::println(elem.get_name());
                for (const auto &bug: elem.get_bugs()) {
                    Utility::colored_txt_output("\tBug name: " + bug.get_name());
                    Utility::colored_txt_output("\tBug description: " + bug.get_description());
                    Utility::colored_txt_output("\tBug severity: " + reinterpret_cast<auto>(bug.get_severity()));
                }
            }
        }
    }

    /**
     * Namespace for different console menus in utility
     */
    namespace Menu {
        /**
        * Menu for situation when user stop test case execution and want to quit with saved progress.
        * Later user might load test cases progress and continue to test.
        * I am truly forget to include this feature earlier (and at the architecture stage).
        */
        [[noreturn]] void stop_menu() {
            int user_action;
            while (true) {
                Utility::println("Stop menu, choose action number to continue:");
                Utility::println("1. Save current progress (create file with progress)");
                Utility::println("2. Load current progress");
                Utility::println("3. See bugs");
                Utility::println("4. Close menu");
                Utility::userInput(user_action);
                switch (user_action) {
                    case 1:
                        Low_level::save_current_progress();
                        break;
                    case 2:
                        Low_level::load_current_progress();
                        break;
                    case 3:
                        Print::see_bugs();
                        break;
                    case 4:
                        break;
                    default:
                        Utility::println("Wrong action number, try again.");
                        continue;
                }
            }
        }

#ifdef EXPERIMENTAL
        [[noreturn]] void run_menu() {
            int user_action;
        }
#endif
    }

    /**
     * Namespace for other things in Check_runner
     */
    namespace Other {
        /**
        * Get cli arguments and add them to vector entity.
        * @param argv string value to scan
        * @return vector with generic objects.
        */
        template<typename T>
        Vec_t<T> resolve_cli_args(const char *argv) {
            if (argv != nullptr) {
                auto vec = Vec_t(arg_count);
                auto split_line = Vec_t<std::string>(arg_count);
                Utility::split(argv, split_line, ' ');
                for (int i = 1; i < arg_count; ++i) {
                    vec.emplace_back(split_line[i]);
                }
                return vec;
            }
            return Vec_t<T>();
        }

        /**
         Function for transferring char array into string array.
         @param char_array some char array to proceed
         @param length len of the array
         @return vector with strings
         */
        Vec_t<std::string> *convert_from_str_2vector(const char *char_array, const int length) {
            const auto str_array = new Vec_t(length);
            for (int i = 0; i < length; ++i) {
                str_array[i].emplace_back(&char_array[i]);
            }
            return str_array;
        }
    }

    /**
    * Main cycle of the utility execution - proceed test cases.
    * @param vts vector for test cases.
    * @param vtr vector for test results.
    * @param device optional device name.
    * @tparam A type of container with test cases
    * @tparam B type of container with test results
    * @throws MainException("Unknown test result."), MainException("Unknown test severity.")
    */
    template<typename A, typename B>
        requires std::derived_from<std::vector<TA::Test_case>, A> and std::derived_from<std::vector<TA::Test_result>, B>
    void main_utility_cycle(const A &vts, B &vtr, const std::string &device = "Single_device_mode") {
        std::chrono::steady_clock::time_point start; //start time of ts execution
        std::chrono::steady_clock::time_point end; //end time of ts execution

        for (const auto &ts: vts) {
            //proceed test case one by one
            Utility::colored_txt_output("Name: " + Utility::trim(ts.get_name()));
            if (Entities::load_parameters->get_is_comments()) {
                Utility::colored_txt_output("Comment: " + ts.get_comment()); //output comments to console
            }
            //get start time of the test case execution
            if (Entities::load_parameters->get_is_time_record()) {
                start = std::chrono::steady_clock::now();
            }
        Back_label:
            //ask user about result block
            std::string action; {
                Utility::colored_txt_output("Is test case successful?");
                Utility::colored_txt_output("You can write 'fluggegecheimen' word to stop utility for actions");
                Utility::colored_txt_output("Enter 'yes' (y), 'no' (n) or 'skip' for test result");

                Utility::print(INPUT_SYM);
                Utility::userInput(action);
                Utility::println(); //just new line symbol
            }

            //result forever loop, if user has not watched "eurotrip" film
            while (true) {
                if (action == stop_word) {
                    Menu::stop_menu();
                    goto Back_label;
                    //Edsger Wybe Dijkstra said that 'goto' statement is bad, I know, but it is some kind of reference to previous version of Test_runner
                }
                //test case result user input
                auto result = TA::Test_result();
                if (action == "yes" or action == "y" or action == "skip") {
                    result.set_name(ts.get_name());
                    result.set_result(action);
                } else if (action == "no" or action == "n") {
                    result.set_name(ts.get_name());
                    result.set_result(action);

                    Utility::println("To exit enter 'exit' word");
                    Utility::println("Write down bugs attributes:");

                    std::string bug_name;
                    std::string bug_description;
                    std::string maybe_bug_severity;

                    while (true) {
                        Utility::print("Enter bug name: ");
                        bug_name = Utility::userInput<std::string>();
                        if (bug_name == EXIT_SYM) {
                            break;
                        }

                        Utility::print("\nEnter bug description (shortly, if you can): ");
                        bug_description = Utility::userInput<std::string>();
                        if (bug_description == EXIT_SYM) {
                            break;
                        }

                        Utility::print("\nEnter bug severity: ");
                        maybe_bug_severity = Utility::userInput<std::string>();
                        if (maybe_bug_severity == EXIT_SYM) {
                            break;
                        }
                        Utility::println(); //simple new line

                        TA::Severity severity;
                        // maybe_bug_severity = TODO строку с большой буквы сделать
                        if (maybe_bug_severity == "Low") {
                            severity = TA::Severity::Low;
                        } else if (maybe_bug_severity == "Medium") {
                            severity = TA::Severity::Medium;
                        } else if (maybe_bug_severity == "High") {
                            severity = TA::Severity::High;
                        } else if (maybe_bug_severity == "Blocker") {
                            severity = TA::Severity::Blocker;
                        } else {
                            //you can write endless loop to not terminate program
                            throw Check_exceptions::MainException("Unknown test severity.");
                        }

                        const auto bug = TA::Bug(bug_name, bug_description, severity);
                        result.add_bug(bug);
                        result.set_device_name(device); //set device name, if not provided - write single_device_mode constant
                        break;
                    }
                    vtr.push_back(result);
                    break;
                } else {
                    //it can be no exception, if you want to not terminate program
                    throw Check_exceptions::MainException("Unknown test result.");
                }
            }
            if (Entities::load_parameters->get_is_time_record()) {
                end = std::chrono::steady_clock::now();
                std::chrono::duration<double> elapsed_seconds = end - start;
                Utility::colored_txt_output("Elapsed seconds for this test case: " + std::to_string(elapsed_seconds.count()));
            }
        }
    }
}

/**
 * Program entry point
 * @param argc count of arguments in utility cli
 * @param args given arguments
 * @throw MainException if necessary files not found
 */
void main(const int argc, const char *args) {
    if (argc > 1) {
        ///Entity init block
        arg_count = argc; {
            Check_runner::Entities::load_parameters = std::make_unique<LP::Load_parameters>();
            Check_runner::Check::check_flags(Check_runner::Other::resolve_cli_args<auto>(args)); //proceed flags to load_parameters structure
            Check_runner::Entities::parser = std::make_unique<Line_interpreter_ns::DirectiveInterpreter>();
        }

        //Pre strategy actions block
        {
            //Modified vector with ts, after all transformations:
            //1) Get data from entry point file
            auto lines_from_file = File_controller::readlines(Check_runner::Entities::load_parameters->get_entry_point());
            Check_runner::Entities::parser->parse_lines_empty(lines_from_file); //2) Delete comments from file
            if (!Check_runner::Entities::load_parameters->get_parameters().empty()) {
                //2.25 Parse given in console parameters
                Check_runner::Entities::parser->parse_parameters(Check_runner::Entities::load_parameters->get_parameters());
            }
            Check_runner::Entities::parser->parse_directives(lines_from_file); //2.5) parse directives in suit file

            if (Check_runner::Entities::load_parameters->get_is_everything_now() && Check_runner::Entities::context->get_strat() == nullptr) {
                Check_runner::Entities::vts = Check_runner::Entities::test_case_fabric->create_test_cases(lines_from_file);
            } else if (!Check_runner::Entities::load_parameters->get_is_everything_now()) {
                //3) Create test cases objects
                Check_runner::Entities::vts = Check_runner::Entities::context->get_strat()->doAlgorithm(
                    Check_runner::Entities::test_case_fabric->create_test_cases(lines_from_file)
                );
            } else {
                Check_runner::Low_level::exit_utility(1, "Error in strategy");
            }
            Check_runner::Entities::vtr = Vec_t<Check_runner::TA::Test_result>();
        }

        //Strategy block
        {
            if (!Check_runner::Entities::load_parameters->get_is_everything_now()) {
                //Execute main cycle of the utility
                if (!Check_runner::Entities::load_parameters->get_devices_entry_point().empty()) {
                    if (Check_runner::Check::check_file_existence(devices_static_file_name)) {
                        const Vec_t devices = File_controller::readlines(Check_runner::Entities::load_parameters->get_devices_entry_point());
                        for (const auto &device: devices) {
                            //get devices from file
                            Utility::colored_txt_output("Device name: " + device);
                            Check_runner::main_utility_cycle(Check_runner::Entities::vts, Check_runner::Entities::vtr, device);
                        }
                    } else {
                        throw Check_exceptions::MainException("No device entry point file was found.");
                    }
                } else {
                    Utility::colored_txt_output("Device name: Single device mode running");
                    Check_runner::main_utility_cycle(Check_runner::Entities::vts, Check_runner::Entities::vtr);
                }
            } else {
                Utility::println("Using 'everything_now strategy' of execution");
                for (const auto &elem: Check_runner::Entities::vts) {
                    // Utility::println(elem); //TODO
                }
            }
        }

        //After strategy block
        {
            //proceed vector with test results at the end of cycle
            Check_runner::Print::print_test_results_2console(Check_runner::Entities::vtr);

            //write test results to file if parameter is true
            if (Check_runner::Entities::load_parameters->get_is_file_write()) {
                Check_runner::Low_level::write_test_results_2file(Check_runner::Entities::vtr);
            }
            Utility::colored_txt_output("Out utility, bye");
        }
    } else if (const std::string conv_arg = args; arg_count == 2 and conv_arg.contains("--help")) {
        //print help to user if user wants help printed
        Check_runner::Print::print_help();
        Check_runner::Low_level::exit_utility(EXIT_SUCCESS);
    } else {
        //anyway, print help to user
        Check_runner::Print::print_help();
        Check_runner::Low_level::exit_utility(EXIT_FAILURE);
    }
}
