#ifndef CHECKLIST_HPP
#define CHECKLIST_HPP

#include <string>
#include <vector>

#include "Test_artifact.hpp"

namespace Check_runner {
    namespace TA {
        /**
         * Entity class for representing checklist in testing.
         */
        class Check_list final : public Test_artifact {
            std::string name;
            std::string description;
            std::vector<std::string> steps;

        public:
            Check_list() = delete;

            Check_list(const std::string &, const std::string &);

            Check_list(const std::string &, const std::string &, const std::vector<std::string> &);

            ~Check_list() override = default;

            [[nodiscard]] std::string get_name() const override;

            [[nodiscard]] std::vector<std::string> get_steps() const;

            [[nodiscard]] std::string get_description() const;

            [[nodiscard]] std::string to_string() const override;
        };
    }
}
#endif
