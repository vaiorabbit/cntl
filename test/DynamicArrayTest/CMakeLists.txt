set( DynamicArrayTest_EXE_HDRS 
    ../common/TestFramework.h
    Vector3.h
    TCDACtorDtor.h
    TCDAResizeReserve.h
    TCDADeletion.h
    TCDAInsertion.h
    TCDAIterator.h
    TCDAAccessor.h
    TCDAOperation.h
    TCDAAlgorithm.h
)

set( DynamicArrayTest_EXE_SRCS 
    DynamicArrayTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( DynamicArrayTest ${DynamicArrayTest_EXE_HDRS} ${DynamicArrayTest_EXE_SRCS} )
add_dependencies( DynamicArrayTest cntl )
target_link_libraries( DynamicArrayTest cntl_lib )

