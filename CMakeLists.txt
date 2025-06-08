cmake_minimum_required(VERSION 3.15)

project(Reelt
    VERSION 0.7
    DESCRIPTION "Reelt: Binary ELF Reverse Engineering tool"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message(STATUS "Host OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

add_subdirectory(libs/CLI11)

set(CAPSTONE_SRC_DIR "${PROJECT_SOURCE_DIR}/libs/capstone")
set(CAPSTONE_BUILD_DIR "${CAPSTONE_SRC_DIR}/build")
set(CAPSTONE_INCLUDE_DIR "${CAPSTONE_SRC_DIR}/include")
set(CAPSTONE_LIBRARY "${CAPSTONE_BUILD_DIR}/libcapstone.a")

if(NOT EXISTS ${CAPSTONE_LIBRARY})
    message(FATAL_ERROR "Capstone static library not found at ${CAPSTONE_LIBRARY}. Build it with CMake in libs/capstone.")
endif()

find_package(OpenSSL REQUIRED)

include_directories(
    ${CAPSTONE_INCLUDE_DIR}
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
    src/version.cpp
)

add_executable(reelt ${SOURCES})

target_link_libraries(reelt
    ${CAPSTONE_LIBRARY}
    CLI11::CLI11
    OpenSSL::Crypto
)

target_include_directories(reelt PRIVATE
    ${CAPSTONE_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/libs/CLI11/include
    ${PROJECT_SOURCE_DIR}/libs/ELFIO
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(reelt PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-parameter)

install(TARGETS reelt DESTINATION bin)
