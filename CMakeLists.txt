cmake_minimum_required(VERSION 3.31)
project(Check_runner)

set(CMAKE_CXX_STANDARD 23)

add_executable(Check_runner
        main.cpp
        Exceptions/Base_exception.hpp
        Exceptions/FileControllerException.hpp
        Exceptions/LineInterpreterException.hpp
        Exceptions/MainException.hpp
        Exceptions/StrategyException.hpp

        Strategies/declaration/High_prior_strat.hpp
        Strategies/declaration/Parallel_strat.hpp
        Strategies/declaration/Random_run_strat.hpp
        Strategies/declaration/Strat.hpp
        Strategies/declaration/StratContex.hpp
        Strategies/realization/High_prior_strat.cpp
        Strategies/realization/Parallel_strat.cpp
        Strategies/realization/Random_run_strat.cpp

        termcolor.hpp

        Test_artifacts/declaration/Bug.hpp
        Test_artifacts/declaration/Check_list.hpp
        Test_artifacts/declaration/Test_artifact.hpp
        Test_artifacts/declaration/Test_case.hpp
        Test_artifacts/declaration/Test_result.hpp
        Test_artifacts/declaration/Test_suit.hpp
        Test_artifacts/realization/Bug.cpp
        Test_artifacts/realization/Check_list.cpp
        Test_artifacts/realization/Test_case.cpp
        Test_artifacts/realization/Test_result.cpp
        Test_artifacts/realization/Test_suit.cpp

        Data/Translation.hpp

        Entities/File_controller.cpp
        Entities/File_controller.hpp
        Entities/LineParser.cpp
        Entities/LineParser.hpp
        Entities/Test_Artifact_Fabric.cpp
        Entities/Test_Artifact_Fabric.hpp

        Load_parameters.hpp
)

target_sources(Check_runner
        PUBLIC
        FILE_SET all_my_modules TYPE CXX_MODULES FILES
        Util_funcs.cppm
)