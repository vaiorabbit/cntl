set( TwoLockQueueTest_EXE_HDRS 
    ../common/TestFramework.h
    TCTLQueue.h
)

set( TwoLockQueueTest_EXE_SRCS 
    TwoLockQueueTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( TwoLockQueueTest ${TwoLockQueueTest_EXE_HDRS} ${TwoLockQueueTest_EXE_SRCS} )
add_dependencies( TwoLockQueueTest cntl )
target_link_libraries( TwoLockQueueTest cntl_lib )

