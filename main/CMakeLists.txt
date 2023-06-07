cmake_minimum_required(VERSION 3.15)

project(open_eex)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    inc/server
    inc/client
    inc/messages
    inc/queue
    external/Catch2/extras
    )

file(GLOB SOURCES src/*/*.cpp)
set(SOURCES ${SOURCES} main.cpp)
message(STATUS "value of source is ${SOURCES}")
add_executable(open_eex ${SOURCES})

set(SOURCES src/queue/priority_queue.cpp src/queue/tests/priority_queue_test.cpp)
message(STATUS "value of source is ${SOURCES}")
add_executable(queue_test ${SOURCES})
find_package(Catch2)
target_link_libraries(${PROJECT_NAME} Catch2::Catch2WithMain)