#ifndef TESTSUIT_HPP
#define TESTSUIT_HPP

#include "Test_artifact.hpp"
#include "Test_case.hpp"

namespace TA {
    /**
     * Class for combine test artifacts.
     * @tparam T generic type for test artifact.
     */
    template<typename T>
    class Test_suit final : public Test_artifact {
        std::string suit_name; ///name of the suit
        std::pmr::vector<T> test_artifacts; ///vector with generic test artifacts

    public:
        ~Test_suit() override;

        Test_suit() = delete;

        explicit Test_suit(const std::string &);

        Test_suit(const std::string &, std::vector<T> &);

        [[nodiscard]] std::string get_name() const override;

        [[nodiscard]] std::string to_string() const override;

        [[nodiscard]] int count_test_artifacts() const;

        void add_test_artifacts(Test_artifact *);

        void delete_test_artifacts(std::string &other);
    };
}

#endif
