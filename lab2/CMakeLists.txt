cmake_minimum_required(VERSION 4.0)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(lab2 main.cpp)
target_link_libraries(lab2 PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(lab2)
