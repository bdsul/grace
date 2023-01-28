# Define required version
# TODO: This could probably be reduced
cmake_minimum_required(VERSION 3.24.0)

# Name the project
project(grace VERSION 0.1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -ggdb)

# Create static library
add_library(${PROJECT_NAME} STATIC "")

# Go into subdirectories to add headers/source files
add_subdirectory(genome)
add_subdirectory(population)
add_subdirectory(grammar)
add_subdirectory(abstract)
add_subdirectory(operators)
add_subdirectory(algorithm)
add_subdirectory(util)

# Include grace.hpp
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER grace.hpp)

# Install static library and header into user's lib and include folders
INSTALL(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include/${PROJECT_NAME})