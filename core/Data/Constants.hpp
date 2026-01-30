#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

//Uncomment above macros to turn on special features:
// #define LIBIO_EXPERIMENTAL //to turn on lib experimental functionality
// #define DEBUG //to turn on debug mode in utility
// #define EXTENDED_FUNCTIONALITY //to turn on utility additional functionality, i.e version 2.0
#define EXTENDED_FUNCTIONALITY_GUI //to turn on graphical interface in utility
// #define EXTENDED_FUNCTIONALITY_IF //turn on if directive

#define DIRECTIVE constexpr auto ///special type for directives in utility
#define JUST_CONSTANT constexpr auto ///special type for constants

#define REPEAT_FOREVER while (true)

//File with utility constants / settings

JUST_CONSTANT version = "1.3.0"; ///version of the app
JUST_CONSTANT comment_sym = "*"; ///use for mark line in suit as a comment
JUST_CONSTANT directive_start_sym = "#"; ///use for mark line in suit as a utility directive
JUST_CONSTANT test_case_separator_sym = "|"; ///symbol for separating modified string line to test case
JUST_CONSTANT stop_word = "fluggegecheimen"; ///creative stop word from "Eurotrip" film to exit from stop menu.
JUST_CONSTANT EXIT_SYM = "exit"; ///symbol for exit in test result input
JUST_CONSTANT INPUT_SYM = ">> "; ///symbol for user input

/**
 * triggers suit creation if : symbol is standing at the end of line and not contain | symbol.
 * Also used in other directives, like if or elif.
 */
DIRECTIVE group_indicator = ":";

//Static file names:
JUST_CONSTANT devices_static_file_name = "devices.txt"; ///static file name of the file with devices to run on
JUST_CONSTANT results_file_name = "results.txt"; ///static file name of the file with test results
JUST_CONSTANT config_file_name = "config.txt";

//Utility directives names:
DIRECTIVE group_directive_start = "Group_start"; ///for group of test cases start, option argument after directive - suit name
DIRECTIVE suit_directive_end = "Group_end"; ///for group end

#ifdef EXTENDED_FUNCTIONALITY_IF
//Branch operators:
#pragma message("Using extended functionality if statement")
DIRECTIVE if_directive = "If"; ///used for selecting branches of test cases, require one argument after directive
DIRECTIVE elif_directive = "Elif"; ///Additional if directive
DIRECTIVE else_directive = "Else"; ///another branch, require one argument after directive
DIRECTIVE endif_directive = "End_if"; ///close if directive in test suit
#endif

DIRECTIVE import_directive = "Import"; ///for importing another suit, require one argument after directive

//For inner test case steps:
DIRECTIVE step_start = "Steps_start"; ///start of the steps in test case comment area
DIRECTIVE step_end = "Steps_end";

//Other directives in utility:
DIRECTIVE ignore_directive = "Ignore"; ///ignore test case with given directive from execution, alternative method to just comment line with *
DIRECTIVE parameters_directive = "Parameters"; ///directive specially for condition checking, example of usage - #Parameters: a=true, b=true

#endif
