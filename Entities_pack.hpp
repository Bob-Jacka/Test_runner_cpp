#ifndef ENTITIES_PACK_HPP
#define ENTITIES_PACK_HPP

#include <memory>
#include <string>
#include <vector>

#include "Load_parameters.hpp"
#include "core/Entities/LineParser.hpp"
#include "core/Entities/Test_Artifact_Fabric.hpp"
#include "core/Strategies/declaration/StratContex.hpp"
#include "core/Test_artifacts/declaration/Test_result.hpp"

template<typename T>
using Vec_t = std::vector<T>; ///short type for vector

using String = std::string; ///short version of std::string

/**
* Structure for entities data in this utility.
* Contains entities for functionality and data for test artifacts.
*/
struct Entities {
    static std::unique_ptr<LP::Load_parameters> load_parameters; ///load parameters of the utility
    static std::unique_ptr<Test_artifact_fabric> test_case_fabric; ///test artifacts fabric entity
    static std::unique_ptr<Strategy::StratContext> context; ///context for determining utility strategy
    static std::unique_ptr<Line_interpreter_ns::DirectiveInterpreter> parser; ///entity for text parsing in suit

    //Vectors with test artifacts:
    static Vec_t<Check_runner::TA::Test_result> vtr; ///vector for test results after test case run
    static Vec_t<Check_runner::TA::Test_case> vts; ///vector for test cases, that created in utility
};

#endif
