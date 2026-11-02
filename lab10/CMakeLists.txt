cmake_minimum_required(VERSION 4.0)
project(lab10)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab10 main.cpp)

include(CTest)
if(BUILD_TESTING)
    find_package(Catch2 3 REQUIRED)

    add_executable(InvertedFileIndexTests InvertedFileIndexTests.cpp)
    target_link_libraries(InvertedFileIndexTests PRIVATE Catch2::Catch2WithMain)
    target_compile_definitions(InvertedFileIndexTests PRIVATE
            ESSAY_FILE="${CMAKE_CURRENT_SOURCE_DIR}/TheAmericanScholar.txt")

    add_executable(InvertedFileIndexBenchmarks InvertedFileIndexBenchmarks.cpp)
    target_link_libraries(InvertedFileIndexBenchmarks PRIVATE Catch2::Catch2WithMain)
    target_compile_definitions(InvertedFileIndexBenchmarks PRIVATE
            ESSAY_FILE="${CMAKE_CURRENT_SOURCE_DIR}/TheAmericanScholar.txt"
            BENCHMARK_CSV="${CMAKE_CURRENT_BINARY_DIR}/inverted_file_index_benchmarks.csv")

    include(Catch)
    catch_discover_tests(InvertedFileIndexTests)
    catch_discover_tests(InvertedFileIndexBenchmarks)
endif()
