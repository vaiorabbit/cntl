// -*- mode: C++; coding: utf-8 -*-
#include <TestFramework.h>

#include "TCSACtorDtor.h"
#include "TCSAIterator.h"
#include "TCSACapacity.h"
#include "TCSAModifier.h"
#include "TCSAOperation.h"
#include "TCSAOperator.h"

int
main( int argc, char** argv )
{
    cntl::MemoryAllocatorProperty property;
    property.m_uAreaSizeForSystemHeap = 8 * 1024 * 1024;
    property.m_uAreaSizeForSTLHeap = 8 * 1024 * 1024;
    cntl::MemoryAllocatorInitialize( property );

    Test::Suite suite( "SortedArray test" );

    Test::Case* tc[] = {
        new TCSACtorDtor( "Ctor/Dtor" ),
        new TCSAIterator( "Iterator" ),
        new TCSACapacity( "Capacity" ),
        new TCSAModifier( "Modifier" ),
        new TCSAOperation( "Operation" ),
        new TCSAOperator( "Operator" ),
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
