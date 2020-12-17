cmake_minimum_required(VERSION 3.16)

project(aoc2019cpp)

find_package(
    Catch2
    CONFIG
    REQUIRED)

find_package(
    fmt
    CONFIG
    REQUIRED)

find_package(
    glm
    CONFIG
    REQUIRED)

find_package(
    range-v3
    CONFIG
    REQUIRED)

add_library(
    aoc2019
    include/aoc2019/aoc2019.hpp
    src/aoc2019.cpp)

add_library(
    esb::aoc2019
    ALIAS
    aoc2019)

target_include_directories(
    aoc2019
    PUBLIC $<INSTALL_INTERFACE:include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/aoc2019
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(aoc2019 PUBLIC cxx_std_20)

target_compile_options(
    aoc2019
    PUBLIC $<$<CXX_COMPILER_ID:MSVC>:-W4
           -WX
           -permissive-
           -analyze>)

include(CTest)
include(Catch)

add_subdirectory(days)
add_subdirectory(tests)
