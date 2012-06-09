set( TokenizerSample_EXE_HDRS 
    TokenizerSample.h
)

set( TokenizerSample_EXE_SRCS 
    TokenizerSample.cpp
)

include_directories( ../../include )

add_executable( TokenizerSample ${TokenizerSample_EXE_HDRS} ${TokenizerSample_EXE_SRCS} )
add_dependencies( TokenizerSample cntl )
target_link_libraries( TokenizerSample cntl_lib )
