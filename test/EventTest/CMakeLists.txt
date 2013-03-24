set( EventTest_EXE_HDRS 
    ../common/TestFramework.h
    TCEvent.h
)

set( EventTest_EXE_SRCS 
    EventTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( EventTest ${EventTest_EXE_HDRS} ${EventTest_EXE_SRCS} )
add_dependencies( EventTest cntl )
target_link_libraries( EventTest cntl_lib )

