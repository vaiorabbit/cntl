set( StringTest_EXE_HDRS 
    ../common/TestFramework.h
    TCStrCtorDtor.h
    TCStrCapacity.h
    TCStrElement.h
    TCStrModifiers.h
    TCStrOperations.h
)

set( StringTest_EXE_SRCS 
    StringTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( StringTest ${StringTest_EXE_HDRS} ${StringTest_EXE_SRCS} )
add_dependencies( StringTest cntl )
target_link_libraries( StringTest cntl_lib )
