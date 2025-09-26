#ifndef TESTSUIT_HPP
#define TESTSUIT_HPP

#include "TestArtifact.hpp"
#include "Test_case.hpp"

namespace TA {
    /**
     * Class for combine test artifacts.
     * @tparam T generic type for test artifact.
     */
    template<typename T = Test_case>
        requires std::derived_from<TestArtifact, T>
    class Test_suit final : public TestArtifact {
        std::string suit_name; ///name of the suit
        std::pmr::vector<T> test_artifacts; ///vector with generic test artifacts

    public:
        ~Test_suit() override;

        Test_suit() = delete;

        explicit Test_suit(const std::string &);

        Test_suit(const std::string &, std::pmr::vector<T> &);

        [[nodiscard]] std::string get_name() const override;

        void to_string() const override;

        void count_test_artifacts() const;

        void add_test_artifacts(TestArtifact *);

        void delete_test_artifacts(std::string &other);
    };
}

#endif
