set( string_symbol_sample_EXE_HDRS 
    string_symbol.h
)

set( string_symbol_sample_EXE_SRCS 
    string_symbol_sample.cpp
)

include_directories( ../../include )

add_executable( string_symbol_sample ${string_symbol_sample_EXE_HDRS} ${string_symbol_sample_EXE_SRCS} )
add_dependencies( string_symbol_sample cntl )
target_link_libraries( string_symbol_sample cntl_lib )
