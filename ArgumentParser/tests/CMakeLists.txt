set(EXECUTABLE_OUTPUT_PATH ../tests/bin)

add_library(
	catch_lib
	catch.hpp
	catch.cpp
	testClass.h
	testClass.cpp
)

target_compile_features(catch_lib PRIVATE cxx_std_17)
target_include_directories(catch_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIRECTORY}")

file(GLOB ARGUMENT_PARSER_TESTS
    catch.cpp
	test_parse_arguments.cpp	
	test_add_argument.cpp
	test_name.cpp
	test_action.cpp
	test_add_parser.cpp
	test_help.cpp
	test_addHelp.cpp
	test_allowAbbrev.cpp
	test_required.cpp
	test_prefixChars.cpp
	test_argumentDefault.cpp
	test_choices.cpp
	test_parseKnownArgs.cpp
)

add_executable(tester ${ARGUMENT_PARSER_TESTS})

target_compile_features(tester PRIVATE cxx_std_17)

target_link_libraries(catch_lib PRIVATE argparser_lib)
target_link_libraries(tester PRIVATE catch_lib)
target_link_libraries(tester PRIVATE argparser_lib)