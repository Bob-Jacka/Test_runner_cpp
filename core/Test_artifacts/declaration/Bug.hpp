#ifndef BUG_HPP
#define BUG_HPP

#include "Test_artifact.hpp"

namespace Check_runner {
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
            Severity    severity;

            public:
                Bug(const std::string &, const std::string &, Severity severity = Severity::Low);

                Bug() = delete;

                Bug(const Bug &) = default;

                ~Bug() override = default;

                [[nodiscard]] std::string get_name() const override;

                [[nodiscard]] std::string get_description() const;

                [[nodiscard]] Severity get_severity() const;

                bool operator<(const Bug &rhs) const;

                bool operator<=(const Bug &rhs) const;

                bool operator>(const Bug &rhs) const;

                bool operator>=(const Bug &rhs) const;

                [[nodiscard]] std::string to_string() const override;
        };

        /**
         * Not a bug, but there is a question due to functionality or visual
         */
        class Question final : Test_artifact {
            std::string name;
            std::string description;

            public:
                Question() = delete;

                Question(const std::string &name, const std::string &description);

                Question(const Question &) = delete;

                [[nodiscard]] std::string get_name() const override;

                [[nodiscard]] std::string to_string() const override;
        };

        /**
         * Not a bug, but some improvement to functionality or visual
         */
        class Enhance final : Test_artifact {
            std::string name;
            std::string description;

            public:
                Enhance() = delete;

                Enhance(const std::string &name, const std::string &description);

                Enhance(const Enhance &) = delete;

                [[nodiscard]] std::string get_name() const override;

                [[nodiscard]] std::string to_string() const override;
        };
    }
}
#endif
