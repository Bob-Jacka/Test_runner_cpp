#ifndef TESTSUIT_HPP
#define TESTSUIT_HPP

#include "TestArtifact.hpp"
#include "Test_case.hpp"

/**
 * Class for combine test artifacts.
 * @tparam T generic type for test artifact.
 */
template<typename T = TA::Test_case>
    requires std::derived_from<TA::TestArtifact, T>
class Test_suit final : public TA::TestArtifact {
    std::string suit_name; ///name of the suit
    std::pmr::vector<T> test_artifacts; ///vector with generic test artifacts

public:
    ~Test_suit() override;

    Test_suit() = delete;

    explicit Test_suit(std::string);

    Test_suit(std::string, std::pmr::vector<T>);

    [[nodiscard]] std::string get_name() const override;

    void to_string() const override;

    void count_test_artifacts() const;

    void add_test_artifacts(TestArtifact *);

    void delete_test_artifacts(std::string &);
};

#endif
