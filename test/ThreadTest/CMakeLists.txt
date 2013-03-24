set( ThreadTest_EXE_HDRS 
    ../common/TestFramework.h
    TCThread.h
)

set( ThreadTest_EXE_SRCS 
    ThreadTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( ThreadTest ${ThreadTest_EXE_HDRS} ${ThreadTest_EXE_SRCS} )
add_dependencies( ThreadTest cntl )
target_link_libraries( ThreadTest cntl_lib )

