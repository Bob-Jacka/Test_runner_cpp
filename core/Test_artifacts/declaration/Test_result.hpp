#ifndef TEST_RESULT_HPP
#define TEST_RESULT_HPP

#include "Bug.hpp"

namespace Check_runner {
    namespace TA {
        class Test_result final : public Test_artifact {
            std::string name;        ///actually test case name
            std::string result;      ///result of running tc
            std::string device_name; ///which device was running
            std::vector<Bug> bugs;        ///available bugs in test case

            std::vector<Question_or_enhance> quest_enhance;

        public:
            Test_result(const std::string &, const std::string &, const std::vector<Bug> &);

            Test_result(const std::string &, const std::string &);

            Test_result();

            ~Test_result() override = default;

            [[nodiscard]] std::vector<Bug> get_bugs() const;

            [[nodiscard]] std::string get_name() const override;

            [[nodiscard]] std::string get_result() const;

            [[nodiscard]] std::string get_device_name() const;

            void set_result(const std::string &);

            void set_bugs(const std::vector<Bug> &);

            void set_name(const std::string &);

            void set_device_name(const std::string &);

            void add_bug(const Bug &);

            void add_quest_enhance(const Question_or_enhance &); ///add question or enhancement in test result

            [[nodiscard]] std::string to_string() const override;
        };
    }
}
#endif
