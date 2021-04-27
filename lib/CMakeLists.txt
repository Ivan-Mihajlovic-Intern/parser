add_library(
	argparser_lib
	ArgumentParser.h
	ArgumentParser.cpp
)

target_compile_features(argparser_lib PRIVATE cxx_std_17)

target_include_directories(argparser_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIRECTORY}")