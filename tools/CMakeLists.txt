add_executable(cat7 cat7.cpp)
target_compile_features(cat7 PUBLIC cxx_std_17)

add_executable(orctool orctool.cpp)
target_link_libraries(orctool PUBLIC orc fmt x595g_core)