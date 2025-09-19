#ifndef TEST_RESULT_HPP
#define TEST_RESULT_HPP

#include <string>

#include "Bug.hpp"

namespace TA {
    class Test_result final : public TestArtifact {
        std::string name; ///actually test case name
        std::string result;
        std::string device_name;
        std::vector<Bug> bugs;

    public:
        Test_result(std::string, std::string, std::vector<Bug>);

        Test_result(std::string, std::string);

        Test_result();

        ~Test_result() override = default;

        [[nodiscard]] std::vector<Bug> get_bugs() const;

        [[nodiscard]] std::string get_name() const override;

        [[nodiscard]] std::string get_description() const;

        [[nodiscard]] std::string get_device_name() const;

        void set_result(std::string);

        void set_bugs(std::vector<Bug>);

        void set_name(std::string);

        void set_device_name(std::string);

        void set_description(std::string);

        void add_bug(Bug);

        void to_string() const override;
    };
}
#endif
