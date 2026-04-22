cmake_minimum_required(VERSION 3.10)

project(List1_ex3)

set(CMAKE_C_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} main2.cpp ${SOURCES})