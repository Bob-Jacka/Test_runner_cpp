#ifndef TESTARTIFACT_HPP
#define TESTARTIFACT_HPP

#include <string>
#include <vector>

namespace TA {
    /**
     * Enum class representing priority of the bug or test case.
     */
    enum class Priority {
        Low = 0,
        Medium = 1,
        High = 2,
        Critical = 3,
    };

    /**
     * Enum class representing severity of the bug or test case.
     */
    enum class Severity {
        Low = 0,
        Medium = 1,
        High = 2,
        Critical = 3,
        Blocker = 4
    };

    /**
     * Abstract class for test artifacts
     */
    class TestArtifact {
    protected:
        std::string name;

    public:
        TestArtifact();

        virtual ~TestArtifact();

        [[nodiscard]] virtual std::string get_name() const = 0;

        virtual void to_string() const = 0;
    };
}

#endif
