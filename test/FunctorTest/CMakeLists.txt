set( FunctorTest_EXE_HDRS 
    ../common/TestFramework.h
    TCFuncCtorDtor.h
)

set( FunctorTest_EXE_SRCS 
    FunctorTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( FunctorTest ${FunctorTest_EXE_HDRS} ${FunctorTest_EXE_SRCS} )
add_dependencies( FunctorTest cntl )
target_link_libraries( FunctorTest cntl_lib )
