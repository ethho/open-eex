cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(open_eex)

add_subdirectory(
    tests
    )

include_directories(
    inc/server
    inc/client
    inc/match
    )

file(GLOB SOURCES src/*/*.cpp)
set(SOURCES ${SOURCES} main.cpp)
message(STATUS "value of source is ${SOURCES}")
add_executable(open_eex ${SOURCES})