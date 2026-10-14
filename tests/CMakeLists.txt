cmake_minimum_required(VERSION 3.5...3.29)
project(CalendarTests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_executable(test_navigation test_navigation.cpp)
target_include_directories(test_navigation PRIVATE ${CMAKE_SOURCE_DIR}/../src)
add_test(NAME navigation COMMAND test_navigation)
