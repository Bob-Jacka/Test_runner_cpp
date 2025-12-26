/**
 * File for translations to other languages.
 * Include Russian and default translation (En)
*/

#ifndef TRANSLATION_HPP
#define TRANSLATION_HPP

// #define RU_TRANSLATION

/**
 * Create your own text translation to your language.
 */
namespace Translation {
#ifdef RU_TRANSLATION
        //Bugs:
        constexpr auto found_bugs = "Найденные баги:";
        constexpr auto bug_name = "\tИмя бага: ";
        constexpr auto bug_desc = "\tОписание бага: ";
        constexpr auto bug_sev = "\tВажность бага: ";
        constexpr auto no_bugs = "Багов нет";

        //Bug severity:
        constexpr auto low = "Низкий";
        constexpr auto medium = "Средний";
        constexpr auto high = "Высокий";
        constexpr auto critical = "Критический";
        constexpr auto blocker = "Блокирующий";

        //Other "bug" words:
        constexpr auto enter_invite = "Введите аттрибуты бага:";
        constexpr auto input_exit = "Для того чтобы выйти введите 'exit' слово";

        //user action:
        constexpr auto accept = "да";
        constexpr auto disaccept = "нет";
        constexpr auto skip = "пропустить";

        //Test case:
        constexpr auto ts_question = "Тест кейс пройден?";
        constexpr auto available_input = "Введите да (д), нет (н) или пропуск для результата проверки";

        //strategies strings:
        constexpr auto high_prior_strat = "Blocker";
        constexpr auto random_strat = "Blocker";
        constexpr auto parallel_strat = "Blocker";

        //other strings:
        constexpr auto elapsed_sec = "Потраченное время на тест кейс: ";

#else // default localization (En)
    constexpr auto device_name = "Device name: ";
    //Bugs
    constexpr auto found_bugs = "Found Bugs:";
    constexpr auto bug_name = "\tBug name: ";
    constexpr auto bug_desc = "\tBug description: ";
    constexpr auto bug_sev = "\tBug severity: ";
    constexpr auto no_bugs = "No bugs";

    //Bug severity:
    constexpr auto low = "Low";
    constexpr auto medium = "Medium";
    constexpr auto high = "High";
    constexpr auto critical = "Critical";
    constexpr auto blocker = "Blocker";

    //Other "bug" words
    constexpr auto enter_invite = "Write down bugs attributes:";
    constexpr auto enter_invite_bug = "Enter bug name: ";
    constexpr auto enter_invite_bug_sev = "\nEnter bug severity: ";
    constexpr auto input_exit = "To exit enter 'exit' word";

    //user action answer
    constexpr auto accept = "yes";
    constexpr auto disaccept = "no";
    constexpr auto skip = "skip";

    //Test case
    constexpr auto ts_question = "Is test case successful?";
    constexpr auto available_input = "Enter yes (y), no (n) or skip for test result";

    //strategies strings
    constexpr auto high_prior_strat = "high_prior";
    constexpr auto random_strat = "random";
    constexpr auto parallel_strat = "parallel";

    //Menu:
    constexpr auto load = "Load";
    constexpr auto save = "Save";

    //other strings
    constexpr auto elapsed_sec = "Elapsed seconds for this test case: ";
#endif
}

#endif
