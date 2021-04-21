set(EXECUTABLE_OUTPUT_PATH ../tests/bin)

add_library(
	catch_lib
	catch.hpp
	catch.cpp
)

add_executable(tester test.cpp)

target_link_libraries(tester PRIVATE catch_lib)
target_link_libraries(tester PRIVATE argparser_lib)