set(SOURCES ArgumentParser.cpp
			pch.cpp)


set(HEADERS ArgumentParser.h
			pch.h)

add_library(argparser_lib ${SOURCES} ${HEADERS})

target_compile_features(argparser_lib PRIVATE cxx_std_17)

target_include_directories(argparser_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIRECTORY}")