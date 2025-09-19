cmake_minimum_required(VERSION 3.31)
project(Check_runner)

set(CMAKE_CXX_STANDARD 23)

add_library(Utils Util_funcs.cppm)

file(GLOB sources
        "*.hpp"
        "*.cpp"
        "*.cppm"
)

add_executable(Check_runner
        main.cpp
        ${sources}
)

target_link_libraries(Check_runner PUBLIC Utils)