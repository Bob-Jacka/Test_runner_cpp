#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define C_AUTO constexpr auto

//File with utility constants / settings

C_AUTO version = "0.0.1"; ///version of the app
C_AUTO comment = "*"; ///use for mark line in suit as a comment
C_AUTO directive = "#"; ///use for mark line in suit as a utility directive
C_AUTO test_case_separator = "|"; ///symbol for separating modified string line to test case
C_AUTO test_suit_indicator = ":"; ///triggers suit creation if : symbol is standing at the end of line and not contain | symbol.

//Static file names:
C_AUTO devices_static_file_name = "devices.txt"; ///static file name of the file with devices to run on
C_AUTO results_file_name = "results.txt"; ///static file name of the file with test results

//Utility directives names:
C_AUTO suit_directive_start = "suit_start"; ///for suit start, option argument after directive - suit name
C_AUTO suit_directive_end = "suit_end"; ///for suit end
C_AUTO if_directive = "if"; ///used for selecting branches of test cases, require one argument after directive
C_AUTO else_directive = "else"; ///another branch, require one argument after directive
C_AUTO endif_directive = "end_if"; ///close if directive
C_AUTO import_directive = "import"; ///for importing another suit, require one argument after directive

#endif
