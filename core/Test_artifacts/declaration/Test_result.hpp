#ifndef TEST_RESULT_HPP
#define TEST_RESULT_HPP

#include "Bug.hpp"

namespace Check_runner {
    namespace TA {
        class Test_result final : public Test_artifact {
            std::string name; ///actually test case name
            std::string result;
            std::string device_name;
            std::vector<Bug> bugs;

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

            [[nodiscard]] std::string to_string() const override;
        };
    }
}
#endif
