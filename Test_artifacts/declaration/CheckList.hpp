#ifndef CHECKLIST_HPP
#define CHECKLIST_HPP
#include <string>
#include <vector>

#include "TestArtifact.hpp"

namespace TA {
    /**
     * Entity class for representing checklist in testing.
     */
    class CheckList final : public TestArtifact {
        std::string name;
        std::string description;
        std::vector<std::string> steps;

    public:
        CheckList() = delete;

        CheckList(std::string, std::string, std::vector<std::string>);

        ~CheckList() override = default;

        [[nodiscard]] std::string get_name() const override;

        [[nodiscard]] std::vector<std::string> get_steps() const;

        void to_string() const override;
    };
}
#endif
