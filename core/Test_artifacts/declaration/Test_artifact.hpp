#ifndef TESTARTIFACT_HPP
#define TESTARTIFACT_HPP

#include <string>
#include <vector>
#include "../../Exceptions/TestArtifactException.hpp"

namespace Check_runner {
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
            throw std::invalid_argument("Invalid priority given - " + priority);
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
            throw std::invalid_argument("Invalid severity given - " + severity);
        }

        /**
         * Proceed string object to Priority value.
         * @param priority string object.
         * @return Priority enum object.
         */
        inline std::string priority_to_object(const Priority priority) {
            if (priority == Priority::Low) {
                return "Low";
            }
            if (priority == Priority::Medium) {
                return "Medium";
            }
            if (priority == Priority::High) {
                return "High";
            }
            if (priority == Priority::Critical) {
                return "Critical";
            }
            throw std::invalid_argument("Invalid priority object was given");
        }

        /**
         * Proceed string object to Severity value.
         * @param severity string object.
         * @return Severity enum object.
         */
        inline std::string object_to_severity(const Severity severity) {
            if (severity == Severity::Low) {
                return "Low";
            }
            if (severity == Severity::Medium) {
                return "Medium";
            }
            if (severity == Severity::High) {
                return "High";
            }
            if (severity == Severity::Critical) {
                return "Critical";
            }
            if (severity == Severity::Blocker) {
                return "Blocker";
            }
            throw std::invalid_argument("Invalid severity object was given");
        }

        /**
         * Abstract class for test artifacts in utility.
         * Base class for test cases, checklists, bugs, test results and test suit
         */
        class Test_artifact {
        protected:
            std::string name;

        public:
            Test_artifact() = default;

            virtual ~Test_artifact() = default;

            [[nodiscard]] virtual std::string get_name() const = 0;

            [[nodiscard]] virtual std::string to_string() const = 0;
        };
    }
}

#endif
