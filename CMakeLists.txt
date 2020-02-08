cmake_minimum_required(VERSION 3.10)

project(x595g)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)

add_library(x595g_core src/instr_type.cpp src/disassembler.cpp src/opcodes.cpp include/x595g/exec.hpp src/exec.cpp include/x595g/memory.hpp)
target_compile_features(x595g_core PUBLIC cxx_std_17)
target_include_directories(x595g_core PUBLIC include)
target_link_libraries(x595g_core PUBLIC fmt Boost::boost)

add_library(orc src/orc.cpp)
target_compile_features(orc PUBLIC cxx_std_17)
target_include_directories(orc PUBLIC include)
target_link_libraries(orc PUBLIC fmt)

add_subdirectory(3rd_party)
add_subdirectory(tools)
add_subdirectory(symbolic)

enable_testing()
add_subdirectory(tests)
