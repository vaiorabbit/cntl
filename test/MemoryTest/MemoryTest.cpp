// -*- mode: C++; coding: utf-8 -*-
#include <vector>

#include <cntl/Memory.h>
#include "TCMemoryDebugAllocator.h"

int
main( int argc, char** argv )
{
    cntl::MemoryAllocatorProperty property;
    property.m_uAreaSizeForSystemHeap = 32 * 1024 * 1024;
    property.m_uAreaSizeForSTLHeap = 8 * 1024 * 1024;
    cntl::MemoryAllocatorInitialize( property );

    Test::Suite suite( "Memory function test" );

    Test::Case* tc[] = {
        new TCMemoryExample( "Example" ),
        new TCAllocator( "STL Memory Allocator" ),
//        new TCMemoryDebugAllocator( "Debug Memory Allocator" ),
    };

    for ( int i = 0; i < sizeof(tc)/sizeof(tc[0]); ++i )
        suite.RegisterCase( tc[i] );

    suite.Run();

    std::cout << Test::ManagerInstance().FailCount() << " of " << Test::ManagerInstance().AssertionCount() << " assertions failed." << std::endl;

    for ( int i = 0; i < sizeof(tc)/sizeof(tc[0]); ++i )
        delete tc[i];

    cntl::MemoryAllocatorFinalize();

    return 0;
}
