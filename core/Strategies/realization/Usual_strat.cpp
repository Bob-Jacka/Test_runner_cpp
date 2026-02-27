#include "../declaration/Usual_strat.hpp"

/**
 * No action, just return input vector.
 * @param input_vector input vector of string lines
 * @return input_vector as output vector
 */
std::vector<Check_runner::TA::Test_case>
Strategy::Usual_strat::doAlgorithm(std::vector<Check_runner::TA::Test_case> &input_vector) const {
    return input_vector;
}
