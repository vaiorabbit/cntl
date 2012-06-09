set( RandomTest_EXE_HDRS 
    ../common/TestFramework.h
    TCRndMonobit.h
)

set( RandomTest_EXE_SRCS 
    RandomTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( RandomTest ${RandomTest_EXE_HDRS} ${RandomTest_EXE_SRCS} )
add_dependencies( RandomTest cntl )
target_link_libraries( RandomTest cntl_lib )
