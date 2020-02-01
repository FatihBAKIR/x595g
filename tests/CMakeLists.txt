add_library(test_main test_main.cpp)
target_include_directories(test_main PUBLIC .)

add_executable(instr_type_test instr_type_test.cpp)
target_link_libraries(instr_type_test PUBLIC x595g_core test_main)
add_test(instr_type_test instr_type_test)


add_executable(by7e_test by7e_test.cpp)
target_link_libraries(by7e_test PUBLIC x595g_core test_main)
add_test(by7e_test by7e_test)