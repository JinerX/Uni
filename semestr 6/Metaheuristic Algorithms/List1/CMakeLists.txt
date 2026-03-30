cmake_minimum_required(VERSION 3.10)

project(List0_ex1)

# Use C standard (optional but recommended)
set(CMAKE_C_STANDARD 11)

# Include header files
include_directories(include)

# Collect all source files from src/
file(GLOB SOURCES "src/*.c")

# Add executable (main.cpp + all src files)
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})