cmake_minimum_required( VERSION 2.8.3 )
project( cntlsample )

include_external_msproject( cntl ${CMAKE_CURRENT_LIST_DIR}/../build/cntl.vcxproj )

# IMPORTED variable :
# http://www.cmake.org/Wiki/CMake_2.6_Notes
add_library( cntl_lib STATIC IMPORTED )
if (CMAKE_HOST_WIN32)
   set_property( TARGET cntl_lib PROPERTY IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_LIST_DIR}/../lib/Debug/cntl.lib)
   set_property( TARGET cntl_lib PROPERTY IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_LIST_DIR}/../lib/Release/cntl.lib)
else (CMAKE_HOST_APPLE)
   set_property( TARGET cntl_lib PROPERTY IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_LIST_DIR}/../lib/Debug/libcntl.a)
   set_property( TARGET cntl_lib PROPERTY IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_LIST_DIR}/../lib/Release/libcntl.a)
endif()

add_subdirectory( Tokenizer )
