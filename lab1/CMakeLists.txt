cmake_minimum_required(VERSION 4.0)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

add_executable(lab1 main.cpp
        random_walk_benchmark.cpp
        random_walk_benchmark.cpp)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(random_walk_benchmark random_walk_benchmark.cpp main.cpp
        random_walk_benchmark.cpp)
target_compile_definitions(random_walk_benchmark PRIVATE RANDOM_WALK_NO_MAIN)
target_link_libraries(random_walk_benchmark PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(random_walk_benchmark)
