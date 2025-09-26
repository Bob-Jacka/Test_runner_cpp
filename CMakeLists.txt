cmake_minimum_required(VERSION 3.31)
project(Check_runner)

set(CMAKE_CXX_STANDARD 23)

file(GLOB sources
        "*.hpp"
        "*.cpp"
)

add_executable(Check_runner
        main.cpp
        ${sources}
)

target_sources(Check_runner
        PUBLIC
        FILE_SET all_my_modules TYPE CXX_MODULES FILES
        Util_funcs.cppm
)