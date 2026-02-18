#include <chrono>
#include <concepts>

#include "Entities_pack.hpp"
#include "core/Data/Translation.hpp"
#include "core/Exceptions/FileControllerException.hpp"
#include "core/Exceptions/MainException.hpp"
#include "core/Strategies/declaration/Usual_strat.hpp"

#ifdef EXTENDED_FUNCTIONALITY
#pragma message("Using extended strategies")

#include "core/Strategies/declaration/High_prior_strat.hpp"
#include "core/Strategies/declaration/Parallel_strat.hpp"
#include "core/Strategies/declaration/Choose_prior_strat.hpp"
#include "core/Strategies/declaration/Random_run_strat.hpp"
#include "core/Entities/Ini/Ini_parser.hpp"

#elifdef EXTENDED_FUNCTIONALITY_GUI
#pragma message("Using graphical user interface mode")
#include "core/GUI/declaration/Elements_callbacks.hpp"
#include "core/GUI/declaration/Main_window.hpp"
#include "FL/Fl_Window.H"
#endif

import UtilFuncs_mod;
import Libio;

/**
 * Special namespace for functions that used in Main function
 */
namespace Check_runner {
#ifdef EXTENDED_FUNCTIONALITY_GUI
    namespace Console {
#endif
        int                                   arg_count = 0;   //count of arguments in utility input
        std::chrono::steady_clock::time_point start_util_work; //start time of utility working
        std::chrono::steady_clock::time_point end_util_work;   //end time of utility working

        /**
         * Low level functionality (filesystem actions) in utility
         */
        namespace Low_level {
            /**
            * Write down test results to file.
            * @param test_results vector with test results.
            * @throw FileControllerException if unknown bug severity
            */
            void write_test_results_2file(const Vec_t<TA::Test_result> &test_results) {
                if (auto file_test_results = File_controller::create_test_result_file(); file_test_results.is_open()) {
                    for (auto &test_result: test_results) {
                        file_test_results << test_result.get_name() << "\n";
                        file_test_results << test_result.get_device_name() << "\n";
                        file_test_results << test_result.get_result() << "\n";
                        if (const auto bugs = test_result.get_bugs(); !bugs.empty()) {
                            int counter = 1; //if you are a programmer - you can start from 0 value!
                            file_test_results << Translation::Console_translation::found_bugs << "\n";
                            for (auto &bug: bugs) {
                                file_test_results << counter << ") " << "\n";
                                file_test_results << "\tBug name: " << bug.get_name() << "\n";
                                file_test_results << "\tBug description: " << bug.get_description() << "\n";
                                file_test_results << "\tBug severity: " << TA::object_to_severity(bug.get_severity()) << "\n";
                                ++counter;
                            }
                        } else {
                            //no bug found branch
                            file_test_results << Translation::Console_translation::no_bugs << "\n";
                        }
                    }
                } else {
                    throw Check_exceptions::FileControllerException(__LINE__, "Error in open file for test results.", __FILE_NAME__);
                }
                libio::output::println("Results written to test results file");
            }

#ifdef EXTENDED_FUNCTIONALITY
            /**
             * Save current progress during utility stop menu
             */
            void save_current_progress();

            void load_current_progress();
#endif
            /**
             * Function for gracefully exit from utility.
             * @param _Code exit code.
             * @param system_msg optional message for message before exit (due to error in utility work)
             */
            void exit_utility(const int _Code, const String &system_msg = "") {
                Entities::vtr.clear();
                Entities::vts.clear();

                if (!system_msg.empty()) {
                    libio::output::println(system_msg);
                } else {
                    libio::output::println("Bye");
                }
                std::exit(_Code);
            }
        }

        /**
         * Namespace for different checks in utility
         */
        namespace Check {
            /**
            * Function for checking command line arguments.
            * @param cont_to_check constant string vector to check for flag value.
            * @throw unknown_compiler_flag if undefined compiler flag occurred.
            */
            void check_flags(String *cont_to_check) {
                ///lambda for checking if string is a flag or not.
                const auto check_func_full = [&](const String &str) -> bool {
                    return str.contains("=") and str.contains("--") and !str.empty();
                };
                using con_string_ref = const std::string &; ///short version of type for string values
                using con_bool_ref   = const bool &;        ///short version of type for boolean values

                if (libio::array::get_dynamic_array_size<std::string>(cont_to_check) != 0) {
                    throw Check_exceptions::MainException(__LINE__, "flags array is empty", __FILE_NAME__);
                }

                //start from 1, because zero index is fullpath to utility executable file
                for (int i = 1; i < arg_count; ++i) {
                    if (auto cli_param = cont_to_check[i]; check_func_full(cli_param)) {
                        auto        split_line = libio::string::split(cli_param, "=");
                        const auto  flag_name  = Utility::replace_string_all(split_line[0], "--", "");
                        std::string flag_value = split_line[1]; //because zero index is flag name

                        //get entry point of utility working
                        if (flag_name == LP::Static_load_parameters_names::entry) {
                            auto entry_point = flag_value;

                            if (File_controller::check_file_existence(entry_point)) {
                                Entities::load_parameters->set_entry_point(entry_point);
                            } else {
                                throw Check_exceptions::MainException(
                                    __LINE__, "Entry point file does not exists in filesystem with name: " + entry_point, __FILE_NAME__);
                            }
                        }
                        //strategies for utility execution:
                        if (flag_name == LP::Static_load_parameters_names::strat) {
                            const auto strat_value = reinterpret_cast<con_string_ref>(flag_value);
                            Entities::context      = std::make_unique<Strategy::StratContext>();

#ifdef EXTENDED_FUNCTIONALITY //strategies disables due to errors in import
#pragma message("Using extended utility strategies")

                            if (strat_value == LP::Static_load_parameters_names::high_prior_strat) {
                                libio::output::println("Using 'high priority' strategy.");
                                Entities::context->set_strategy(
                                    std::make_unique<Strategy::High_prior_strat>()
                                );
                            } else if (strat_value == LP::Static_load_parameters_names::random_strat) {
                                libio::output::println("Using 'pseudo run' strategy.");
                                Entities::context->set_strategy(
                                    std::make_unique<Strategy::Random_run_strat>()
                                );
                            } else if (strat_value == LP::Static_load_parameters_names::parallel_strat) [[unlikely]] {
                                libio::output::println("Using 'parallel' strategy.");
                                Entities::context->set_strategy(
                                    std::make_unique<Strategy::Parallel_strat>()
                                );
                            } else if (strat_value == LP::Static_load_parameters_names::choose_strat) {
                                libio::output::println("Using 'choose' strategy.");
                                Entities::context->set_strategy(
                                    std::make_unique<Strategy::Choose_prior_strat>()
                                );
                            }

                            else
#endif
                            if (strat_value == LP::Static_load_parameters_names::usual_strat) [[likely]] {
                                libio::output::println("Using 'usual' strategy.");
                                Entities::context->set_strategy(
                                    std::make_unique<Strategy::Usual_strat>()
                                );
                            } else {
                                throw Check_exceptions::MainException(__LINE__, "No strategy selected for value: " + strat_value, __FILE_NAME__);
                            }
                        }
                        //other utility flags block
                        {
                            //set utility usage parameters:
                            if (flag_name == LP::Static_load_parameters_names::parameters) {
                                Entities::load_parameters->set_parameters(flag_name);
                            }

                            //turn on graphical user interface:
                            if (flag_name == LP::Static_load_parameters_names::inter) [[likely]] {
                                Entities::load_parameters->set_is_gui(libio::convert::str2bool(flag_value));
                            }

                            //need for more devices than one:
                            if (flag_name == LP::Static_load_parameters_names::devices) {
                                Entities::load_parameters->set_devices_entry_point(flag_value);
                            }

                            //time check flag:
                            if (flag_name == LP::Static_load_parameters_names::time_check) {
                                Entities::load_parameters->set_is_time_record(libio::convert::str2bool(flag_value));
                            }

                            //colored flag:
                            if (flag_name == LP::Static_load_parameters_names::colored) [[likely]] {
                                Entities::load_parameters->set_is_colored(libio::convert::str2bool(flag_value));
                            }

                            //comments flag:
                            if (flag_name == LP::Static_load_parameters_names::comments) [[likely]] {
                                Entities::load_parameters->set_is_comments(libio::convert::str2bool(flag_value));
                            }
                        }
                    } else {
                        throw Check_exceptions::MainException(__LINE__, "Unknown compiler flag selected: " + cli_param, __FILE_NAME__);
                        //kill utility if unknown flag detected
                    }
                }
                //Apply default strategy if no strategy was given
#ifdef USUALSTRAT_HPP
                if (Entities::context == nullptr) {
                    Entities::context = std::make_unique<Strategy::StratContext>();
                    Entities::context->set_strategy(
                        std::make_unique<Strategy::Usual_strat>()
                    );
                }
#elifndef USUALSTRAT_HPP
#error "Usual strategy should be defined in utility context"
#endif
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
                    libio::output::println("№ " + counter);
                    libio::output::println("Test name: " + res.get_name());
                    libio::output::println("Test result: " + res.get_result());
                    if (const auto &bugs_vec = res.get_bugs(); !bugs_vec.empty()) {
                        for (const auto &bug: bugs_vec) {
                            libio::output::println("\tBug name: " + bug.get_name());
                            libio::output::println("\tBug description: " + bug.get_description());
                            libio::output::println("\tBug severity: " + TA::object_to_severity(bug.get_severity()));
                        }
                    } else {
                        libio::output::println("No bugs in " + res.get_name());
                    }
                    ++counter;
                }
                libio::output::println(); //just new line
                const std::chrono::duration<double> util_work_time = end_util_work - start_util_work;
                const auto                          minutes{std::chrono::duration_cast<std::chrono::minutes>(util_work_time)};
                const auto                          hours{std::chrono::duration_cast<std::chrono::hours>(util_work_time)};

                libio::output::println("Hours: " + std::to_string(hours.count()));
                libio::output::println("Minutes: " + std::to_string(minutes.count()));
            }

            /**
            * Function for printing help to user.
            */
            void print_help() {
                libio::output::println("Test runner C++ edition");
                libio::output::println("Utility description: - utility is using for test cycles\n");
                libio::output::println("Utility parameters:");
                libio::output::println("\t1. 'strategy' - used for specifying run strategy");
#ifdef EXTENDED_FUNCTIONALITY
                libio::output::println("\t\t1.1 'high_prior' - for only high priority test run");
                libio::output::println("\t\t1.2 'parallel' - for parallel random adding test");
                libio::output::println("\t\t1.3 'random' - for pseudo-random strategy mode");
                libio::output::println("\t\t1.4 'everything_now' - manual ts execution, print ts in console");
#endif
                libio::output::println("\t\t1.5 'usual' - usual mode");
#ifdef EXTENDED_FUNCTIONALITY
                libio::output::println("\t\t1.6 'choose' - choose test cases with which severity to run");
#endif
                libio::output::println("\t2. 'devices' - (optional) for providing devices names to test cycle");
                libio::output::println("\t3. 'time_record' - for record time, during test case execution");
                libio::output::println("\t4. 'colored' - for color text output");
                libio::output::println("\t5. 'comments' - for test case comment output\n");
                libio::output::println("\t6. 'inter' - for utility interface (can be 'gui' or 'console')\n");
                libio::output::println("Available utility directives in files with tests:");
                libio::output::println("\t1. 'Group_start' - directive for indication for test suit start,");
                libio::output::println("\t2. 'Group_end' - directive for indication for test suit end,");
                libio::output::println("\t3. 'If' - usual if operator for branching in many programming languages,");
                libio::output::println("\t4. 'Else_if' - alternative branch,");
                libio::output::println("\t5. 'Else' - additional directive for if");
                libio::output::println("\t6. 'End_if' - closes if condition");
            }

            /**
             * Function for printing bugs during test run (not for save in file or print in console)
             */
            void see_bugs() {
                for (const auto &elem: Entities::vtr) {
                    libio::output::println(elem.get_name());
                    for (const auto &bug: elem.get_bugs()) {
                        libio::output::println("\tBug name: " + bug.get_name());
                        libio::output::println("\tBug description: " + bug.get_description());
                        libio::output::println("\tBug severity: " + TA::object_to_severity(bug.get_severity()));
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
                REPEAT_FOREVER {
                    libio::output::colored::colored_print("Stop menu, choose action number to continue:", "\n",
                                                          libio::output::colored::Ansi_colors::MAGENTA);
#ifdef EXTENDED_FUNCTIONALITY
                 Utility::println("1. Save / Load menu");
#endif
                    libio::output::println("2. See bugs");
                    libio::output::println("3. Close menu");
                    USER_INPUT(user_action);
                    switch (user_action) {
#ifdef EXTENDED_FUNCTIONALITY
                    case 1:
                            save_load_menu();
                            break;
#endif
                        case 2:
                            Print::see_bugs();
                            break;
                        case 3:
                            break;
                        default:
                            libio::output::println("Wrong action number, try again.");
                    }
                }
            }
#ifdef EXTENDED_FUNCTIONALITY
            /**
             * Menu for bug entering
             */
            [[noreturn]] void bug_menu() {
            int user_action;
            REPEAT_FOREVER {
                libio::output::println();
                Utility::userInput(user_action);
                switch (user_action) {
                    //
                    }
                }
            }

            [[noreturn]] void run_menu() {
                int user_action;
                REPEAT_FOREVER {
                    libio::output::println();
                    Utility::userInput(user_action);
                    switch (user_action) {
                        //
                    }
                }
            }

            /**
             * Menu for save or load functionality
             */
            [[noreturn]] void save_load_menu() {
                int user_action;
                REPEAT_FOREVER {
                    libio::output::println("1. Save current progress");
                    libio::output::println("2. Load progress");
                    libio::output::println("3. Close menu");
                    Utility::userInput(user_action);
                    switch (user_action) {
                        case 1:
                            save_current_progress();
                        case 2:
                            load_current_progress();
                        case 3:
                            break
                        default:
                            continue
                    }
                }
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
            * @throw MainException if cannot resolve arguments in command line interface
            * @return vector with generic objects.
            */
            String *resolve_cli_args(char *argv[]) {
                if (argv != nullptr) {
                    auto *str_arr = libio::array::create_1d_array<std::string>(arg_count);
                    for (int i = 0; i < arg_count; ++i) {
                        str_arr[i] = argv[i];
                    }
                    return str_arr;
                }
                throw Check_exceptions::MainException(__LINE__, "Cannot resolve CLI arguments, because null", __FILE_NAME__);
            }

            /**
             Function for transferring char array into string array.
             @param char_array some char array to proceed
             @param length len of the array
             @return vector with strings
             */
            Vec_t<String> *convert_from_str_2vector(const char *char_array, const int length) {
                const auto str_array = new Vec_t<String>(length);
                for (int i = 0; i < length; ++i) {
                    str_array[i].emplace_back(&char_array[i]);
                }
                return str_array;
            }
        }

        /**
        * Main cycle of the utility execution - proceed test cases.
        * @param vtc vector for test cases.
        * @param vtr vector for test results.
        * @param device optional device name.
        * @tparam A type of container with test cases
        * @tparam B type of container with test results
        * @throws MainException("Unknown test result"), MainException("Unknown test severity")
        */
        template<typename A, typename B>
            requires std::derived_from<std::vector<TA::Test_case>, A> and std::derived_from<std::vector<TA::Test_result>, B>
        void main_utility_cycle(const A &vtc, B &vtr, const String &device = "Single_device_mode") {
            using namespace std::chrono;
            steady_clock::time_point start; //start time of ts execution
            steady_clock::time_point end;   //end time of ts execution

            const bool is_comment = Entities::load_parameters->get_is_comments();
            const bool is_time    = Entities::load_parameters->get_is_time_record();

            start_util_work = steady_clock::now();
            for (const auto &ts: vtc) {
                //proceed test case one by one:
                libio::output::println_w(L"Name: " + libio::output::to_wstring(ts.get_name()));
                if (is_comment) {
                    libio::output::println_w(L"Comment: " + libio::output::to_wstring(ts.get_comment())); //output comments to console
                }
                if (is_time) {
                    start = steady_clock::now(); //get start time of the test case execution:
                }
            Ask_again_label:
                //ask user about result block
                String result; {
                    libio::output::colored::colored_println("Is test case successful?",
                                                            libio::output::colored::Ansi_colors::GREEN);
                    libio::output::colored::colored_println("You can write 'fluggegecheimen' word to stop utility for actions",
                                                            libio::output::colored::Ansi_colors::CYAN);
                    libio::output::colored::colored_println("Optionally - Question or Enhance",
                                                            libio::output::colored::Ansi_colors::CYAN);
                    libio::output::colored::colored_println("Enter 'yes' (y), 'no' (n) or 'skip' for test result",
                                                            libio::output::colored::Ansi_colors::CYAN);

                    USER_INPUT(result);
                }

                //result - forever loop, if user has not watched "eurotrip" film
                REPEAT_FOREVER {
                    if (result == stop_word) {
                        Menu::stop_menu();
                        libio::output::println("After pause:");
                        goto Ask_again_label;
                        //Edsger Wybe Dijkstra said that 'goto' statement is bad, I know, but it is some kind of reference to previous version of Test_runner
                    }
                    //test case result user input
                    auto test_res = TA::Test_result(); {
                        if (result == Translation::Console_translation::accept or result == Translation::Console_translation::accept_short) {
                            test_res.set_name(ts.get_name());
                            test_res.set_result(Translation::Console_translation::suc_res); //apply success result
                        } else if (result == Translation::Console_translation::question or result == Translation::Console_translation::enhance) {
                            auto quest_or_enhance = TA::Question_or_enhance();
                            TA::enter_question_or_enhance(quest_or_enhance);
                            test_res.set_name(ts.get_name()); //if Question or Enhance
                            test_res.set_result(Translation::Console_translation::skip_res);
                            test_res.add_quest_enhance(quest_or_enhance);
                            vtr.push_back(test_res); //add question or enhance to test results
                            goto Ask_again_label;    //Return back, because user inputted question or enhance, but not test result
                        } else if (result == Translation::Console_translation::skip) {
                            test_res.set_name(ts.get_name());
                            test_res.set_result(Translation::Console_translation::skip_res);
                        } else if (result == Translation::Console_translation::disaccept or result ==
                                   Translation::Console_translation::disaccept_short) {
                            test_res.set_name(ts.get_name());
                            test_res.set_result(Translation::Console_translation::fail_res);

                            libio::output::colored::colored_print("To exit enter 'exit' word", "\n",
                                                                  libio::output::colored::Ansi_colors::CYAN);
                            libio::output::println(Translation::Console_translation::enter_invite);

                            String bug_name;
                            String bug_description;
                            String maybe_bug_severity;

                            REPEAT_FOREVER {
                                libio::output::print(Translation::Console_translation::enter_invite_bug);
                                USER_INPUT(bug_name);
                                if (bug_name == EXIT_SYM) {
                                    break;
                                }

                                libio::output::print("\nEnter bug description (shortly, if you can): ");
                                USER_INPUT(bug_description);
                                if (bug_description == EXIT_SYM) {
                                    break;
                                }

                                libio::output::print(Translation::Console_translation::enter_invite_bug_sev);
                                USER_INPUT(maybe_bug_severity);
                                if (maybe_bug_severity == EXIT_SYM) {
                                    break;
                                }
                                libio::output::println(); //simple new line

                                TA::Severity severity;
                                maybe_bug_severity = libio::string::change_string_register(maybe_bug_severity, true);
                                if (maybe_bug_severity == Translation::Console_translation::low) {
                                    severity = TA::Severity::Low;
                                } else if (maybe_bug_severity == Translation::Console_translation::medium) {
                                    severity = TA::Severity::Medium;
                                } else if (maybe_bug_severity == Translation::Console_translation::high) {
                                    severity = TA::Severity::High;
                                } else if (maybe_bug_severity == Translation::Console_translation::blocker) {
                                    severity = TA::Severity::Blocker;
                                } else {
                                    //you can write endless loop to not terminate program
                                    throw Check_exceptions::MainException(__LINE__, "Unknown test severity.", __FILE_NAME__);
                                }

                                const auto bug = TA::Bug(bug_name, bug_description, severity);
                                test_res.add_bug(bug);
                                test_res.set_device_name(device); //set device name, if not provided - write single_device_mode constant
                                break;
                            }
                            vtr.push_back(test_res);
                            break;
                        } else {
                            //it can be no exception, if you want to not terminate the program (simply write endless loop or write continue)
                            throw Check_exceptions::MainException(__LINE__, "Unknown test result.", __FILE_NAME__);
                        }
                    }
                    Entities::vtr.push_back(test_res);
                    break;
                }
                if (is_time) {
                    end                              = steady_clock::now();
                    duration<double> elapsed_seconds = end - start;
                    libio::output::println(Translation::Console_translation::elapsed_sec + std::to_string(elapsed_seconds.count()));
                }
            }
            end_util_work = steady_clock::now();
        }
    }

#ifdef EXTENDED_FUNCTIONALITY_GUI
    namespace GUI {
        // Style table, 1 - color, 2 - font_type, 3 - font size | 4 attr, 5 bg color
        Fl_Text_Display::Style_Table_Entry _styletable[] = {
            {FL_BLACK, FL_COURIER, FONT_SIZE},                           // A - Plain simple text
            {FL_GRAY, FL_COURIER_ITALIC, COMMENT_FONT_SIZE, 0, FL_GRAY}, // B - Line comments
            {FL_GRAY, FL_COURIER_ITALIC, COMMENT_FONT_SIZE},             // C - Block comments
            {FL_BLUE, FL_COURIER_BOLD, FONT_SIZE},                       // D - Strings (Parameters)
            {FL_DARK_YELLOW, FL_COURIER | FL_BOLD, DIRECTIVE_FONT_SIZE}, // E - Directives
            {FL_BLACK, FL_COURIER_BOLD, FONT_SIZE},                      // F - Types, !disable strings with default color
            {FL_DARK_RED, FL_COURIER_BOLD, FONT_SIZE}                    // G - Keywords
        };

        /**
         * Build window menu with callbacks
         * @param menu menu object to build
         * @param window window object
         */
        static void build_menu(Fl_Menu_Bar *menu, Fl_Window *window) {
            static bool counter = true;
            try {
                const Fl_Menu_Item menuItems[] =
                {
                    {"&File", 0, nullptr, nullptr, FL_SUBMENU}, //file menu
                    {"&New file", FL_COMMAND + 'n', reinterpret_cast<Fl_Callback *>(new_callback), window},
                    {"&Open file", FL_COMMAND + 'o', reinterpret_cast<Fl_Callback *>(open_callback), window},
                    {"&Save file", FL_COMMAND + 's', reinterpret_cast<Fl_Callback *>(save_callback), window},
                    {"&Save file as", FL_COMMAND + FL_SHIFT + 's', reinterpret_cast<Fl_Callback *>(save_as_сallback), window, FL_MENU_DIVIDER},
                    {"&Exit", FL_COMMAND + 'q', reinterpret_cast<Fl_Callback *>(exit_сallback)},
                    {nullptr},
                    {"&Edit", 0, nullptr, nullptr, FL_SUBMENU}, //edit menu
                    {"&Undo", FL_COMMAND + 'z', reinterpret_cast<Fl_Callback *>(undo_сallback), window, FL_MENU_DIVIDER},
                    {"&Cut", FL_COMMAND + 'x', cut_сallback, window},
                    {"&Copy", FL_COMMAND + 'c', copy_сallback, window},
                    {"&Paste", FL_COMMAND + 'v', paste_сallback, window},
                    {"&Delete", 0, reinterpret_cast<Fl_Callback *>(delete_сallback)},
                    {nullptr},
                    {"&Search", 0, nullptr, nullptr, FL_SUBMENU}, //search menu
                    {"&Find", FL_COMMAND + 'f', find_сallback, window},
                    {"&Find again", FL_COMMAND + 'g', find_2_сallback, window},
                    {"&Replace", FL_COMMAND + 'r', replace_сallback, window},
                    {"&Replace again", FL_COMMAND + 't', replace_2_сallback, window},
                    {nullptr},
                    {"&Help", 0, nullptr, nullptr, FL_SUBMENU}, //help menu
                    {"&About", FL_COMMAND + 'h', about_callback, window, FL_MENU_DIVIDER},
                    {"&Increase font", FL_ALT + '+', increase_font_callback, window},
                    {"&Decrease font", FL_ALT + '-', decrease_font_callback, window},
                    {nullptr},
                    {"&AI", 0, nullptr, nullptr, FL_SUBMENU}, //AI menu
                    {"&Chat", 0, AI_chat_callback, window},
                    {"&Agent", 0, AI_agent_callback, window, FL_MENU_DIVIDER},
#ifdef GUI_LANG_CHECKER
                    {"&Check grammar", 0, AI_check_grammar, window},
#endif
                    {nullptr},
                    {nullptr}
                };
                menu->copy(menuItems);
            } catch (...) {
#ifdef DEBUG
                printf("An error in creating menu: build_menu function");
#endif
                if (counter == true) {
                    //try to create second time
                    build_menu(menu, window);
                }
                counter = false;
            }
        }

        /**
         * Create main window
         * @return pointer to window
         */
        Fl_Window *create_view() noexcept {
            const auto window       = new Main_window(800, 600, "Check runner");
            const auto style_buffer = new Fl_Text_Buffer(); //text buffer for styles
            window->begin();                                //start creating main window of utility

            const auto menuBar = new Fl_Menu_Bar(0, 0, 800, 30);

            window->m_editor = new Fl_Text_Editor(10, 30, 780, 560);
            window->m_editor->buffer(window->m_text_buffer);

            build_menu(menuBar, window);

            window->m_replace_dlg->hide(); //hide dialog by default
            window->end();                 //end creating main window

            window->resizable(window->m_editor);
            window->m_editor->highlight_data(style_buffer, _styletable, std::size(_styletable), 'A', nullptr, nullptr);
            window->m_editor->linenumber_width(60);

            window->m_text_buffer->add_modify_callback(changed_сallback, window);
            window->m_text_buffer->add_modify_callback(colorize_callback, window);
            window->m_text_buffer->add_modify_callback(grammar_nazi_check, window);
            window->m_text_buffer->call_modify_callbacks();
            return window;
        }
    }
}
#endif

/**
 * Program entry point
 * @param argc count of arguments in utility cli
 * @param argv given arguments to utility
 * @param envp environmental arguments
 * @throw MainException if necessary files not found
 */
int main(int argc, char *argv[]
#ifdef EXTENDED_FUNCTIONALITY
, char *envp[]
#endif
) {
    using namespace Check_runner;

#ifdef EXTENDED_FUNCTIONALITY
Extended_begin_label:
#endif

    if (argc > 1) {
        ///Entity init block
        Console::arg_count = argc; {
            Entities::load_parameters = std::make_unique<LP::Load_parameters>();
            Console::Check::check_flags(Console::Other::resolve_cli_args(argv)); //proceed flags first, before parser

#ifdef EXTENDED_FUNCTIONALITY_GUI
#ifdef GUI_LANG_CHECKER
            Entities::corrector.LoadLangModel("model.bin");
#endif
            //1) Run graphical user interface before creating other entities
            if (Entities::load_parameters->get_gui()) {
                const auto window = GUI::create_view();
                window->show();
                return Fl::run();
            }
#endif
            Entities::parser = std::make_unique<Interpreter_ns::Directive_interpreter>();
        }

        //Pre strategy actions block
        {
            bool global_strat_state = false; //global state of everything_now strategy to not use load_parameters
            //1) Get data from entry point file
            auto lines_from_file = File_controller::readlines(Entities::load_parameters->get_entry_point());
            global_strat_state   = Entities::load_parameters->get_is_everything_now();

            lines_from_file = Entities::parser->preprocess_lines(lines_from_file); //2) Delete comments from file
            if (not Entities::load_parameters->get_parameters().empty()) {
                //2.25 Parse given in console parameters
                Entities::parser->parse_parameters(Entities::load_parameters->get_parameters());
            }
            //Modified vector with ts, after all transformations:
            lines_from_file = Entities::parser->parse_directives(lines_from_file); //2.5) parse directives in suit file
            if (global_strat_state and Entities::context->get_strat() == nullptr) {
                Entities::vts = Entities::test_case_fabric->create_test_cases(lines_from_file);
            } else if (not global_strat_state) {
                try {
                    //3) Create test cases objects
                    Entities::vts = Entities::context->get_strat()->doAlgorithm(
                        Entities::test_case_fabric->create_test_cases(lines_from_file)
                    );
                } catch (...) {
                    throw Check_exceptions::MainException(__LINE__, "Strategy execution error", __FILE_NAME__);
                }
            } else {
                //exit utility with strategy error
                Console::Low_level::exit_utility(EXIT_FAILURE, "Error in strategy execution at line: " + __LINE__);
            }
            Entities::vtr = Vec_t<TA::Test_result>();
            //Strategy block
            {
                if (not global_strat_state) {
                    //Execute main cycle of the utility
                    auto devices_file_name = Entities::load_parameters->get_devices_entry_point();
                    if (not devices_file_name.empty()) {
                        if (File_controller::check_file_existence(devices_file_name)) {
                            for (const auto &device: File_controller::readlines(Entities::load_parameters->get_devices_entry_point())) {
                                //get devices from file
                                libio::output::colored::colored_print(Translation::Console_translation::device_name + device, "\n",
                                                                      libio::output::colored::Ansi_colors::YELLOW);
                                Console::main_utility_cycle(Entities::vts, Entities::vtr, device); //one utility cycle for one device
                            }
                        } else {
                            throw Check_exceptions::MainException(
                                __LINE__, "No device entry point file was found - " + std::string(devices_file_name), __FILE_NAME__);
                        }
                    } else {
                        libio::output::colored::colored_print(
                            Translation::Console_translation::device_name + std::string("Single device mode running"), "\n",
                            libio::output::colored::Ansi_colors::YELLOW);
                        Console::main_utility_cycle(Entities::vts, Entities::vtr);
                    }
                }
#ifdef EXTENDED_FUNCTIONALITY
                else {
                    Utility::println("Using 'everything_now strategy' of execution");
                    for (const auto &elem: Entities::vts) {
                        libio::output::println(elem);
                    }
                }
#endif
            }

            //After strategy block
            {
                //proceed vector with test results at the end of cycle
                Console::Print::print_test_results_2console(Entities::vtr);

                //write test results to file if parameter is true
                if (Entities::load_parameters->get_is_file_write()) {
                    Console::Low_level::write_test_results_2file(Entities::vtr);
                }
            }
        }
    } else if (const String conv_arg = *argv; Console::arg_count == 2 and conv_arg.contains(LP::Static_load_parameters_names::help)) {
        //print help to user if user wants help to be printed
        Console::Print::print_help();
        Console::Low_level::exit_utility(EXIT_SUCCESS);
    }
#ifdef EXTENDED_FUNCTIONALITY
            //Config read branch
            else {
                if (File_controller::check_file_existence(config_file_name)) {
                    auto config_lines = File_controller::readlines(config_file_name);
                    const auto ini_parser = std::make_unique<Interpreter_ns::Ini_parser>(config_lines);
                    //increment arguments count and return back
                    argc = ini_parser->get_section_count();
                    argv = ini_parser->convert_to_char_array();
                    goto Extended_begin_label;
                }
                //anyway, print help to user
                Print::print_help();
                Low_level::exit_utility(EXIT_SUCCESS);
            }
#endif
    libio::output::println("Out utility, bye");
}

#ifndef EXTENDED_FUNCTIONALITY_GUI
}
#endif
