cmake_minimum_required(VERSION 3.20)
project(Parser VERSION 1.0.0)



option(BUILD_DOC "Build documentation" ON)


find_package( Doxygen )

if ( DOXYGEN_FOUND )

message("-- Doxygen found")

set( DOXYGEN_OUTPUT_DIRECTORY doxygen )
#set( DOXYGEN_COLLABORATION_GRAPH YES )
set( DOXYGEN_EXTRACT_ALL YES )
set( DOXYGEN_RECURSIVE YES )
#set( DOXYGEN_CLASS_DIAGRAMS YES )
set( DOXYGEN_HIDE_UNDOC_RELATIONS NO )
#set( DOXYGEN_HAVE_DOT YES )
#set( DOXYGEN_CLASS_GRAPH YES )
#set( DOXYGEN_CALL_GRAPH YES )
#set( DOXYGEN_CALLER_GRAPH YES )
#set( DOXYGEN_COLLABORATION_GRAPH YES )
set( DOXYGEN_BUILTIN_STL_SUPPORT YES )
set( DOXYGEN_EXTRACT_PACKAGE YES )
set( DOXYGEN_EXTRACT_LOCALMETHODS YES )
set( DOXYGEN_UML_LOOK YES )
set( DOXYGEN_UML_LIMIT_NUM_FIELDS 50 )
set( DOXYGEN_TEMPLATE_RELATIONS YES )
#set( DOXYGEN_DOT_GRAPH_MAX_NODES 100 )
#set( DOXYGEN_MAX_DOT_GRAPH_DEPTH 0 )
#set( DOXYGEN_DOT_TRANSPARENT YES )
set( DOXYGEN_EXTRACT_PRIVATE NO )
set( DOXYGEN_ENABLE_PREPROCESSING YES )
set( DOXYGEN_MACRO_EXPANSION YES )
set( DOXYGEN_EXPAND_ONLY_PREDEF YES )
set( DOXYGEN_PREDEFINED protected=private )
#set( DOXYGEN_SORT_BRIEF_DOCS YES )
set( DOXYGEN_GENERATE_TREEVIEW YES )
set( DOXYGEN_DISBALE_INDEX YES )
set( DOXYGEN_EXCLUDE ${CMAKE_SOURCE_DIR}/ArgumentParser/tests)
#set( DOXYGEN_DOT_PATH "C:/Program Files/Graphviz/bin")

doxygen_add_docs( doxygen ALL
				${CMAKE_SOURCE_DIR}
				)
message("-- Docs added")
else()

message( "Doxygen need to be installed to generate the doxygen documentation" )

endif()


add_subdirectory(ArgumentParser)
add_subdirectory(ArgumentParserLib)