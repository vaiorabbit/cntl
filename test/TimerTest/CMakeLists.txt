set( TimerTest_EXE_HDRS 
    ../common/TestFramework.h
    TCTimer.h
)

set( TimerTest_EXE_SRCS 
    TimerTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( TimerTest ${TimerTest_EXE_HDRS} ${TimerTest_EXE_SRCS} )
add_dependencies( TimerTest cntl )
target_link_libraries( TimerTest cntl_lib )

