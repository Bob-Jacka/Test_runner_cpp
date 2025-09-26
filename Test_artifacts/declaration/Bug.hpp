#ifndef BUG_HPP
#define BUG_HPP
#include <string>

#include "TestArtifact.hpp"

/**
 * Namespace for all test artifacts
 */
namespace TA {
    /**
     * Data class for bug entity.
     */
    class Bug final : TestArtifact {
        std::string name;
        std::string description;
        Severity severity;

    public:
        Bug(std::string name, std::string description, Severity severity = Severity::Low);

        Bug() = delete;

        Bug(const Bug &) = default;

        ~Bug() override = default;

        [[nodiscard]] std::string get_name() const override;

        [[nodiscard]] std::string get_description() const;

        [[nodiscard]] Severity get_severity() const;

        Bug operator<=>(Bug &other) const;

        void to_string() const override;
    };
}
#endif
