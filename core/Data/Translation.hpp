/**
 * File for translations to other languages
 */

#ifndef TRANSLATION_HPP
#define TRANSLATION_HPP

// #define EN_TRANSLATION
// #define RU_TRANSLATION

/**
 * Create your own Translation namespace with text translation to your language.
 */
namespace Translation {
#ifdef RU_TRANSLATION
        consteval auto found_bugs = "Найденные баги:";
        consteval auto bug_name = "\tИмя бага: ";
        consteval auto bug_desc = "\tОписание бага: ";
        consteval auto bug_sev = "\tВажность бага: ";

        //Bug severity:
        consteval auto low = "Низкий";
        consteval auto medium = "Средний";
        consteval auto high = "Высокий";
        consteval auto critical = "Критический";
        consteval auto blocker = "Блокирующий";

        //Other "bug" words
        consteval auto enter_invite = "Введите аттрибуты бага:";
        consteval auto input_exit = "To exit enter 'exit' word";

        //user action
        consteval auto accept = "да";
        consteval auto disaccept = "нет";
        consteval auto skip = "пропустить";

        ////Test case
        consteval auto ts_question = "Тест кейс пройден?";
        consteval auto available_input = "Введите да (д), нет (н) или пропуск для результата проверки";

        ////strategies strings
        consteval auto high_prior_strat = "Blocker";
        consteval auto random_strat = "Blocker";
        consteval auto parallel_strat = "Blocker";

        ////other strings
        consteval auto elapsed_sec = "Elapsed seconds for this test case: ";

#elifdef EN_TRANSLATION
        consteval auto found_bugs = "Found Bugs:";
        consteval auto bug_name = "\tBug name: ";
        consteval auto bug_desc = "\tBug description: ";
        consteval auto bug_sev = "\tBug severity: ";

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

#else
    consteval auto found_bugs = "Found Bugs:";
    consteval auto bug_name = "\tBug name: ";
    consteval auto bug_desc = "\tBug description: ";
    consteval auto bug_sev = "\tBug severity: ";

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
#endif
}

#endif
