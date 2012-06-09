set( SortedArrayTest_EXE_HDRS 
    ../common/TestFramework.h
    Vector3.h
    TCSACtorDtor.h
    TCSAIterator.h
    TCSACapacity.h
    TCSAModifier.h
    TCSAOperation.h
    TCSAOperator.h
)

set( SortedArrayTest_EXE_SRCS 
    SortedArrayTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( SortedArrayTest ${SortedArrayTest_EXE_HDRS} ${SortedArrayTest_EXE_SRCS} )
add_dependencies( SortedArrayTest cntl )
target_link_libraries( SortedArrayTest cntl_lib )
