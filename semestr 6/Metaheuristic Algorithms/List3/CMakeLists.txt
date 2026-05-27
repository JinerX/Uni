cmake_minimum_required(VERSION 3.14)
project(ga_tsp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-O2)

find_package(OpenMP)

add_executable(ga main.cpp)

if(OpenMP_CXX_FOUND)
    target_link_libraries(ga OpenMP::OpenMP_CXX)
    message(STATUS "OpenMP found — parallelization enabled")
else()
    message(STATUS "OpenMP not found — parallelization disabled")
endif()
