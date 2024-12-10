cmake_minimum_required (VERSION 3.21)

project(DMITI)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

include_directories(${PROJECT_NAME} src)

add_subdirectory(src)