cmake_minimum_required(VERSION 3.10.0)
project(AOD_ZAD3 VERSION 0.1.0 LANGUAGES C CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCES "src/*.cpp" "src/*.c")

add_executable(AOD_ZAD3 main.cpp ${SOURCES})