cmake_minimum_required(VERSION 3.10)
project(x595g)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(z3)

add_executable(symbolic main.cpp)
target_link_libraries(symbolic PUBLIC z3)
