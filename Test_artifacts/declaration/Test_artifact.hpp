#ifndef TESTARTIFACT_HPP
#define TESTARTIFACT_HPP

#include <string>
#include <vector>

namespace TA {
    /**
     * Enum class representing priority of the bug or test case.
     * Priority to execute test case or priority to fix bug.
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
     * Proceed string object to Priority value.
     * @param priority string object.
     * @return Priority enum object.
     */
    inline Priority priority_to_object(const std::string &priority) {
        if (priority == "Low") {
            return Priority::Low;
        }
        if (priority == "Medium") {
            return Priority::Medium;
        }
        if (priority == "High") {
            return Priority::High;
        }
        if (priority == "Critical") {
            return Priority::Critical;
        }
        throw;
    }

    /**
     * Proceed string object to Severity value.
     * @param severity string object.
     * @return Severity enum object.
     */
    inline Severity severity_to_object(const std::string &severity) {
        if (severity == "Low") {
            return Severity::Low;
        }
        if (severity == "Medium") {
            return Severity::Medium;
        }
        if (severity == "High") {
            return Severity::High;
        }
        if (severity == "Critical") {
            return Severity::Critical;
        }
        if (severity == "Blocker") {
            return Severity::Blocker;
        }
        throw;
    }

    /**
     * Abstract class for test artifacts in utility.
     * Base class for test cases, checklists, bugs, test results and test suit
     */
    class Test_artifact {
    protected:
        std::string name;

    public:
        Test_artifact();

        virtual ~Test_artifact();

        [[nodiscard]] virtual std::string get_name() const = 0;

        [[nodiscard]] virtual std::string to_string() const = 0;
    };
}

#endif
