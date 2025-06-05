cmake_minimum_required(VERSION 3.15)

project(Reelt
    VERSION 0.1
    DESCRIPTION "Reelt: Binary ELF Injector/Editor with Capstone"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Host OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAPSTONE REQUIRED capstone)

if(NOT CAPSTONE_FOUND)
    message(FATAL_ERROR "Capstone library not found. Install it using your package manager (e.g., sudo apt install libcapstone-dev)")
endif()

add_subdirectory(libs/CLI11)

include_directories(
    ${CAPSTONE_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/libs/CLI11/include
    ${PROJECT_SOURCE_DIR}/libs/ELFIO
    ${PROJECT_SOURCE_DIR}/include
)

set(SOURCES
    src/main.cpp
    src/CLI.cpp
    src/utils.cpp
    src/elf_parser.cpp
    src/disassembler.cpp
    src/editor.cpp
    src/injector.cpp
)

add_executable(reelt ${SOURCES})

target_link_libraries(reelt
    ${CAPSTONE_LIBRARIES}
    CLI11::CLI11
)

target_include_directories(reelt PRIVATE
    ${CAPSTONE_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/libs/CLI11/include
    ${PROJECT_SOURCE_DIR}/libs/ELFIO
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(reelt PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-parameter)

install(TARGETS reelt DESTINATION bin)
