cmake_minimum_required(VERSION 3.5...3.29)
project(Interface)

find_package(pybind11 REQUIRED)  # or `add_subdirectory(pybind11)`

add_executable(Interface Interface.cpp)
target_link_libraries(Interface PRIVATE pybind11::embed)