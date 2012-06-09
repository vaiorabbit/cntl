set( MemoryTest_EXE_HDRS 
    ../common/TestFramework.h
    TCMemoryDebugAllocator.h
)

set( MemoryTest_EXE_SRCS 
    MemoryTest.cpp
)

include_directories( ../../include )
include_directories( ../common )

add_executable( MemoryTest ${MemoryTest_EXE_HDRS} ${MemoryTest_EXE_SRCS} )
add_dependencies( MemoryTest cntl )
target_link_libraries( MemoryTest cntl_lib )
