set( JobQueueSample_EXE_HDRS 
    JobQueue.h
)

set( JobQueueSample_EXE_SRCS 
    JobQueue.cpp
	JobQueueSample.cpp
)

include_directories( ../../include )

add_executable( JobQueueSample ${JobQueueSample_EXE_HDRS} ${JobQueueSample_EXE_SRCS} )
add_dependencies( JobQueueSample cntl )
target_link_libraries( JobQueueSample cntl_lib )
