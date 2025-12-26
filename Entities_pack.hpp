#ifndef ENTITIES_PACK_HPP
#define ENTITIES_PACK_HPP

#include <string>
#include <vector>

#include "Load_parameters.hpp"
#include "core/Entities/Line/LineParser.hpp"
#include "core/Entities/Test_Artifact_Fabric.hpp"
#ifdef EXTENDED_FUNCTIONALITY
#include "core/Entities/Ini_parser.hpp"
#endif
#include "core/Strategies/declaration/StratContex.hpp"
#include "core/Test_artifacts/declaration/Test_result.hpp"

template<typename T>
using Vec_t = std::vector<T>; ///short type for vector with generic type

using String = std::string; ///short version of std::string

namespace Check_runner {
    /**
* Structure for entities data in this utility.
* Contains entities for functionality and data for test artifacts.
*/
    struct Entities {
        //Other entities:
        static std::unique_ptr<LP::Load_parameters> load_parameters; ///load parameters of the utility
        static std::unique_ptr<TA::Test_artifact_fabric> test_case_fabric; ///test artifacts fabric entity
        static std::unique_ptr<Strategy::StratContext> context; ///context for determining utility strategy
        static std::unique_ptr<Interpreter_ns::DirectiveInterpreter> parser; ///entity for text parsing in suit

        //Vectors with test artifacts:
        static Vec_t<TA::Test_result> vtr; ///vector for test results after test case run
        static Vec_t<TA::Test_case> vts; ///vector for test cases, that created in utility
    };

#ifdef EXTENDED_FUNCTIONALITY

    /**
     * Group of test cases
     */
    struct TS_group {
    private:
        std::string group_id; ///id of the group
        Vec_t<Check_runner::TA::Test_case> tests_in_group;

    public:
        TS_group() = delete;

        ~TS_group() = default;

        explicit TS_group(const Vec_t<Check_runner::TA::Test_case> &tests_in_group) {
            this->tests_in_group = tests_in_group;
        }

        void add_test(const Check_runner::TA::Test_case &new_test_line) {
            tests_in_group.push_back(new_test_line);
        }
    };

    /**
     * Group of groups of tests cases
     */
    struct TS_groups {
    private:
        std::unique_ptr<std::map<std::string, TS_group> > groups; ///groups of groups, key - group id, value - ts_group with test cases
        std::string group_parameters; ///parameters directive data in group

    public:
        TS_groups() = delete;

        ~TS_groups() = default;

        explicit TS_groups(const std::string &group_parameters = "") {
            this->groups = std::make_unique<std::map<std::string, TS_group> >();
            this->group_parameters = group_parameters;
        }

        void add_group(const std::string &group_id, const TS_group &group) const {
            this->groups->insert(std::make_pair(group_id, group));
        }
    };
#endif
}

#endif
