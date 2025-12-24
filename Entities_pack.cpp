#include "Entities_pack.hpp"

std::unique_ptr<LP::Load_parameters> Check_runner::Entities::load_parameters = nullptr;
std::unique_ptr<Check_runner::TA::Test_artifact_fabric> Check_runner::Entities::test_case_fabric = nullptr;
std::unique_ptr<Strategy::StratContext> Check_runner::Entities::context = nullptr;
std::unique_ptr<Interpreter_ns::DirectiveInterpreter> Check_runner::Entities::parser = nullptr;

Vec_t<Check_runner::TA::Test_result> Check_runner::Entities::vtr = {};

#ifndef EXTENDED_FUNCTIONALITY
Vec_t<Check_runner::TA::Test_case> Check_runner::Entities::vts = {};
#elifdef EXTENDED_FUNCTIONALITY
Vec_t<Check_runner::TS_group> Entities::vts = {};
std::unique_ptr<IniParser> ini_parser = nullptr;
#endif
