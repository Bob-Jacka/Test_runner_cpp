cmake_minimum_required(VERSION 3.31)
project(Check_runner)

set(CMAKE_CXX_STANDARD 23)

if (DEFINED TEST_INCLUDE)
    add_executable(Check_runner
            Tests/Test_dependencies/catch_amalgamated.hpp
            Tests/Test_dependencies/catch_amalgamated.cpp
            Tests/Tests.cpp
    )
endif ()

add_executable(Check_runner
        core/Data/Constants.hpp
        core/Data/Translation.hpp
        core/Exceptions/Base_exception.hpp
        core/Exceptions/FileControllerException.hpp
        core/Exceptions/LineInterpreterException.hpp
        core/Exceptions/MainException.hpp
        core/Exceptions/StrategyException.hpp
        core/Exceptions/TestArtifactException.hpp

        core/Entities/File_controller.cpp
        core/Entities/File_controller.hpp
        core/Entities/Line/LineParser.cpp
        core/Entities/Line/LineParser.hpp
        core/Entities/Test_Artifact_Fabric.cpp
        core/Entities/Test_Artifact_Fabric.hpp

        Entities_pack.cpp
        Entities_pack.hpp

        #        core/Strategies/declaration/Choose_prior_strat.hpp
        #        core/Strategies/declaration/High_prior_strat.hpp
        #        core/Strategies/declaration/Parallel_strat.hpp
        #        core/Strategies/declaration/Random_run_strat.hpp

        core/Strategies/declaration/Strat.hpp
        core/Strategies/declaration/StratContex.hpp

        core/Strategies/declaration/Usual_strat.hpp
        core/Strategies/realization/Usual_strat.cpp
        #        core/Strategies/realization/Choose_prior_strat.cpp
        #        core/Strategies/realization/High_prior_strat.cpp
        #        core/Strategies/realization/Parallel_strat.cpp
        #        core/Strategies/realization/Random_run_strat.cpp

        core/Test_artifacts/declaration/Test_artifact.hpp
        core/Test_artifacts/declaration/Bug.hpp
        core/Test_artifacts/declaration/Check_list.hpp
        core/Test_artifacts/declaration/Test_case.hpp
        core/Test_artifacts/declaration/Test_result.hpp
        core/Test_artifacts/declaration/Test_suit.hpp
        core/Test_artifacts/realization/Bug.cpp
        core/Test_artifacts/realization/Check_list.cpp
        core/Test_artifacts/realization/Test_case.cpp
        core/Test_artifacts/realization/Test_result.cpp
        modules/libio.cppm
        modules/Util_funcs.cppm
        main.cpp
        Load_parameters.hpp
)

target_sources(Check_runner
        PUBLIC
        FILE_SET all_my_modules TYPE CXX_MODULES FILES
        modules/Util_funcs.cppm
        modules/libio.cppm
)