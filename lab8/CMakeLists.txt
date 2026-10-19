cmake_minimum_required(VERSION 4.0)
project(lab8)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 3 REQUIRED)
include(CTest)
include(Catch)

add_executable(lab8 main.cpp
        JobQueue.cpp
        JobQueue.h)
target_link_libraries(lab8 PRIVATE Catch2::Catch2WithMain)

catch_discover_tests(lab8)
