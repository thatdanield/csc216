cmake_minimum_required(VERSION 4.0)
project(lab9)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab9 main.cpp
        HashMap.cpp
        HashMap.h)

include(CTest)
if(BUILD_TESTING)
    find_package(Catch2 3 REQUIRED)

    configure_file(RomeoAndJuliet.txt RomeoAndJuliet.txt COPYONLY)
    configure_file(Othello.txt Othello.txt COPYONLY)

    add_executable(HashMapTests
            HashMapTests.cpp
            HashMap.cpp
            HashMap.h)
    target_link_libraries(HashMapTests PRIVATE Catch2::Catch2WithMain)

    add_executable(NoteTests
            NoteTests.cpp
            main.cpp
            HashMap.cpp
            HashMap.h)
    target_link_libraries(NoteTests PRIVATE Catch2::Catch2WithMain)
    target_compile_definitions(NoteTests PRIVATE NOTE_TESTS)

    add_executable(NoteBenchmarks
            NoteBenchmarkTests.cpp
            main.cpp
            HashMap.cpp
            HashMap.h)
    target_link_libraries(NoteBenchmarks PRIVATE Catch2::Catch2WithMain)
    target_compile_definitions(NoteBenchmarks PRIVATE NOTE_TESTS)

    include(Catch)
    catch_discover_tests(HashMapTests)
    catch_discover_tests(NoteTests)
    catch_discover_tests(NoteBenchmarks)
endif()
