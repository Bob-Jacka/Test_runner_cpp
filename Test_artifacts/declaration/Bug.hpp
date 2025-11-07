#ifndef BUG_HPP
#define BUG_HPP
#include <string>

#include "Test_artifact.hpp"

/**
 * Namespace for all test artifacts in testing
 */
namespace TA {
    /**
     * Data class for bug entity.
     */
    class Bug final : Test_artifact {
        std::string name;
        std::string description;
        Severity severity;

    public:
        Bug(const std::string& name, const std::string& description, Severity severity = Severity::Low);

        Bug() = delete;

        Bug(const Bug &) = default;

        ~Bug() override = default;

        [[nodiscard]] std::string get_name() const override;

        [[nodiscard]] std::string get_description() const;

        [[nodiscard]] Severity get_severity() const;

        Bug operator<=>(Bug &other) const;

        [[nodiscard]] std::string to_string() const override;
    };
}
#endif
