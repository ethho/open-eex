cmake_minimum_required(VERSION 3.15)

project(open_eex)

add_subdirectory(
    tests
    )

include_directories(
    inc/server
    inc/client
    )

file(GLOB SOURCES src/*/*.cpp)
set(SOURCES ${SOURCES} main.cpp)
message(STATUS "value of source is ${SOURCES}")
add_executable(open_eex ${SOURCES})