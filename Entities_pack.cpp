#include "Entities_pack.hpp"

std::unique_ptr<LP::Load_parameters> Entities::load_parameters = nullptr;
std::unique_ptr<Test_artifact_fabric> Entities::test_case_fabric = nullptr;
std::unique_ptr<Strategy::StratContext> Entities::context = nullptr;
std::unique_ptr<Line_interpreter_ns::DirectiveInterpreter> Entities::parser = nullptr;

Vec_t<Check_runner::TA::Test_result> Entities::vtr = {};
Vec_t<Check_runner::TA::Test_case> Entities::vts = {};
