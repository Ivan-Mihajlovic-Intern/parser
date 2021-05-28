set(EXECUTABLE_OUTPUT_PATH ../usage/bin)

add_executable(main main.cpp)

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE argparser_lib)