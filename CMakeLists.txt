cmake_minimum_required(VERSION 3.13)

project(aco_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

add_subdirectory(src)
include_directories("src")

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries (${PROJECT_NAME} aco)

add_subdirectory (tests)
