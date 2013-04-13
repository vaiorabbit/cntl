set( AssocArrayTest_EXE_HDRS 
    ../common/TestFramework.h
    Vector3.h
    TCAACtorDtor.h
    TCAAIterator.h
    TCAACapacity.h
    TCAAModifier.h
    TCAAOperation.h
    TCAAOperator.h
    TCAAPerformance.h
)

set( AssocArrayTest_EXE_SRCS 
    AssocArrayTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( AssocArrayTest ${AssocArrayTest_EXE_HDRS} ${AssocArrayTest_EXE_SRCS} )
add_dependencies( AssocArrayTest cntl )
target_link_libraries( AssocArrayTest cntl_lib )
