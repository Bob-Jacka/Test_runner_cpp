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
            Severity severity;

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
         * Task type
         */
        enum Type {
            QUESTION,
            ENHANCE,
        };

        /**
         * Not a bug, but there is a question due to functionality or visual;
         * Not a bug, but some improvement to functionality or visual
         */
        class Question_or_enhance final : Test_artifact {
            std::string name;
            std::string description;
            Type type;

        public:
            friend void enter_question_or_enhance(Question_or_enhance &object);

            Question_or_enhance();

            Question_or_enhance(const std::string &name, const std::string &description, Type type);

            Question_or_enhance(const Question_or_enhance &) = default;

            Question_or_enhance(const Question_or_enhance &&) = delete;

            Question_or_enhance &operator=(const Question_or_enhance &other) = delete;

            [[nodiscard]] std::string get_name() const override;

            [[nodiscard]] std::string to_string() const override;

            [[nodiscard]] std::string get_description() const;

            [[nodiscard]] Type get_type() const;
        };

        void enter_question_or_enhance(Question_or_enhance &object);
    }
}
#endif
