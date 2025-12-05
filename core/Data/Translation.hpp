/**
 * File for translations to other languages
 */

#ifndef TRANSLATION_HPP
#define TRANSLATION_HPP

/**
 * Create your own Translation namespace with text translation to your language.
 */
namespace Translation {
#ifdef RU_TRANSLATION
    namespace RU {
        //
    }
#endif

#if EN_TRANSLATION
    namespace EN {
        consteval auto found_bugs = "Found Bugs:";
        consteval auto bug_name = "\tBug name: ";
        consteval auto bug_desc = "\tBug description: ";
        consteval auto bug_sev = "\tBug severity: ";

        ////Bug entry:
        //Bug severity:
        consteval auto low = "Low";
        consteval auto medium = "Medium";
        consteval auto high = "High";
        consteval auto critical = "Critical";
        consteval auto blocker = "Blocker";

        //Other "bug" words
        consteval auto enter_invite = "Write down bugs attributes:";
        consteval auto input_exit = "To exit enter 'exit' word";

        //user action
        consteval auto accept = "yes";
        consteval auto disaccept = "no";
        consteval auto skip = "skip";

        ////Test case
        consteval auto ts_question = "Is test case successful?";
        consteval auto available_input = "Enter yes (y), no (n) or skip for test result";

        ////strategies strings
        consteval auto high_prior_strat = "Blocker";
        consteval auto random_strat = "Blocker";
        consteval auto parallel_strat = "Blocker";

        ////other strings
        consteval auto elapsed_sec = "Elapsed seconds for this test case: ";
    }
#endif
}

#endif
