cmake_minimum_required(VERSION 4.0)
project(HelloVerilog)

set(CMAKE_CXX_STANDARD 20)

add_executable(HelloVerilog main.cpp)

find_program(SLANG_EXECUTABLE
    NAMES slang
    HINTS /opt/homebrew/bin /usr/local/bin
    REQUIRED
)

set_source_files_properties(blank.sv PROPERTIES HEADER_FILE_ONLY TRUE)
target_sources(HelloVerilog PRIVATE blank.sv)

add_custom_target(check_systemverilog
    COMMAND "${SLANG_EXECUTABLE}" --std 1800-2017 "${CMAKE_CURRENT_SOURCE_DIR}/blank.sv"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/blank.sv"
    COMMENT "Checking SystemVerilog with ${SLANG_EXECUTABLE}"
    VERBATIM
)
