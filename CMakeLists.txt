cmake_minimum_required( VERSION 2.8.3 )
project( cntl )

file( GLOB CNTL_LIB_HDRS "${CMAKE_CURRENT_LIST_DIR}/include/cntl/*.h" )
file( GLOB CNTL_LIB_SRCS "${CMAKE_CURRENT_LIST_DIR}/source/*.cpp" )
add_library( cntl ${CNTL_LIB_HDRS} ${CNTL_LIB_SRCS} )
include_directories( ${CMAKE_CURRENT_LIST_DIR}/include )

set( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/lib )
