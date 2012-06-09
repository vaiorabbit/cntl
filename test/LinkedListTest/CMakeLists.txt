set( LinkedListTest_EXE_HDRS 
    ../common/TestFramework.h
    Vector3.h
    TCLLCtorDtor.h
    TCLLModifier.h
    TCLLCapacity.h
    TCLLIterator.h
    TCLLAlgorithm.h
    TCLLOperator.h
)

set( LinkedListTest_EXE_SRCS 
    LinkedListTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( LinkedListTest ${LinkedListTest_EXE_HDRS} ${LinkedListTest_EXE_SRCS} )
add_dependencies( LinkedListTest cntl )
target_link_libraries( LinkedListTest cntl_lib )
