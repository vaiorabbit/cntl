// -*- mode: C++; coding: utf-8 -*-
#include <vector>

#include <cntl/Memory.h>
#include <cntl/STLAllocator.h>

#include "TCStrCtorDtor.h"
#include "TCStrCapacity.h"
#include "TCStrElement.h"
#include "TCStrModifiers.h"
#include "TCStrOperations.h"

#include <TestFramework.h>

int
main( int argc, char** argv )
{
    cntl::MemoryAllocatorProperty property;
    property.m_uAreaSizeForSystemHeap = 8 * 1024 * 1024;
    property.m_uAreaSizeForSTLHeap = 8 * 1024 * 1024;
    cntl::MemoryAllocatorInitialize( property );

    Test::Suite suite( "String test" );

    Test::Case* tc[] = {
        new TCStrCtorDtor( "Ctor/Dtor" ),
        new TCStrCapacity( "Capacity" ),
        new TCStrElement( "Element access" ),
        new TCStrModifiers( "Modifiers" ),
        new TCStrOperations( "Operations" ),
    };

    for ( int i = 0; i < sizeof(tc)/sizeof(tc[0]); ++i )
        suite.RegisterCase( tc[i] );

    suite.Run();

    if ( Test::ManagerInstance().FailCount() == 0 )
        std::cout << Test::ManagerInstance().AssertionCount() << " assertions succeeded." << std::endl;
    else
        std::cout << Test::ManagerInstance().FailCount() << " of " << Test::ManagerInstance().AssertionCount() << " assertions failed." << std::endl;

    for ( int i = 0; i < sizeof(tc)/sizeof(tc[0]); ++i )
        delete tc[i];

    cntl::MemoryAllocatorFinalize();

    return 0;
}
