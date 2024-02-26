cmake_minimum_required(VERSION 3.13)

project(aco_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
add_subdirectory(src)

add_executable(${PROJECT_NAME} bin/main.cpp)
target_link_libraries (${PROJECT_NAME} aco)