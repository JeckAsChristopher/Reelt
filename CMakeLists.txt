cmake_minimum_required(VERSION 3.15)
project(Reelt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAPSTONE REQUIRED capstone)

add_subdirectory(deps/CLI11)

include_directories(
    ${CAPSTONE_INCLUDE_DIRS}
    deps/CLI11/include
    deps/ELFIO
    include
)

add_executable(reelt
    src/main.cpp
    src/CLI.cpp
    src/utils.cpp
    src/elf_parser.cpp
    src/disassembler.cpp
)

target_link_libraries(reelt
    ${CAPSTONE_LIBRARIES}
    CLI11::CLI11
)

target_include_directories(reelt PRIVATE
    ${CAPSTONE_INCLUDE_DIRS}
    deps/CLI11/include
    deps/ELFIO
    include
)

target_compile_options(reelt PRIVATE -Wall -Wextra -Wpedantic)
