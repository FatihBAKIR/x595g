add_library(test_main test_main.cpp)
target_include_directories(test_main PUBLIC .)
target_compile_features(test_main PUBLIC cxx_std_17)

add_executable(instr_type_test instr_type_test.cpp)
target_link_libraries(instr_type_test PUBLIC x595g_core test_main)
add_test(instr_type_test instr_type_test)

add_executable(by7e_test by7e_test.cpp)
target_link_libraries(by7e_test PUBLIC x595g_core test_main)
add_test(by7e_test by7e_test)

add_executable(disas_test disas_test.cpp)
target_link_libraries(disas_test PUBLIC x595g_core test_main)
add_test(disas_test disas_test)