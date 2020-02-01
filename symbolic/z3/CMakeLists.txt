project(z3)

find_package(Threads REQUIRED)

add_library(z3 INTERFACE)
target_link_libraries(z3 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/bin/libz3.a)
target_link_libraries(z3 INTERFACE Threads::Threads)
target_include_directories(z3 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
message(STATUS "Z3 Imported")