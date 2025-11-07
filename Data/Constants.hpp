#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define DIRECTIVE constexpr auto ///special type for directives in utility

//File with utility constants / settings

DIRECTIVE version = "0.0.3"; ///version of the app
DIRECTIVE comment = "*"; ///use for mark line in suit as a comment
DIRECTIVE directive_start = "#"; ///use for mark line in suit as a utility directive
DIRECTIVE test_case_separator = "|"; ///symbol for separating modified string line to test case

/**
 * triggers suit creation if : symbol is standing at the end of line and not contain | symbol.
 * Also used in other directives
 */
DIRECTIVE group_indicator = ":";

//Static file names:
DIRECTIVE devices_static_file_name = "devices.txt"; ///static file name of the file with devices to run on
DIRECTIVE results_file_name = "results.txt"; ///static file name of the file with test results

//Utility directives names:
DIRECTIVE group_directive_start = "Group_start"; ///for group of test cases start, option argument after directive - suit name
DIRECTIVE suit_directive_end = "Group_end"; ///for group end

//Branch operators:
DIRECTIVE if_directive = "If"; ///used for selecting branches of test cases, require one argument after directive
DIRECTIVE elif_directive = "Elif"; ///Additional if directive
DIRECTIVE else_directive = "Else"; ///another branch, require one argument after directive
DIRECTIVE endif_directive = "End_if"; ///close if directive in test suit
DIRECTIVE import_directive = "Import"; ///for importing another suit, require one argument after directive

//For inner test case steps:
DIRECTIVE step_start = "Steps_start"; ///start of the steps in test case comment area
DIRECTIVE step_end = "Steps_end";

//Other directives:
DIRECTIVE ignore_directive = "Ignore"; ///ignore test case with given directive from execution, alternative method to just comment line with *
DIRECTIVE parameters_directive = "Parameters"; ///directive specially for condition checking, example of usage - #Parameters: a=true, b=true

#endif
