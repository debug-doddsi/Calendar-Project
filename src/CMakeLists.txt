cmake_minimum_required(VERSION 3.5...3.29)
project(Calendar)

include_directories(${CMAKE_SOURCE_DIR}/../include)   # Point to the RGB matrix header files
link_directories   (${CMAKE_SOURCE_DIR}/../lib)       # Point to the RGB matrix lib stuff

find_package  (pybind11 REQUIRED)                     # Locating the pybind stuff (it is globally installed)
add_executable(Calendar Calendar.cpp)                 # Adding the code

target_link_libraries(  
                        Calendar                      # Point to the Calendar.o
                        pybind11::embed               # Point to pybind11
                        rgbmatrix                     # Point to where the RGB matrix stuff lives
                        pthread                       # Added bc it was complaining without it :)
                     )