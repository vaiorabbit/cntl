// -*- mode: C++; coding: utf-8 -*-
#include <TestFramework.h>

#include "TCAACtorDtor.h"
#include "TCAAIterator.h"
#include "TCAACapacity.h"
#include "TCAAModifier.h"
#include "TCAAOperation.h"
#include "TCAAOperator.h"
#include "TCAAPerformance.h"

int
main( int argc, char** argv )
{
    cntl::MemoryAllocatorProperty property;
    property.m_uAreaSizeForSystemHeap = 8 * 1024 * 1024;
    property.m_uAreaSizeForSTLHeap = 8 * 1024 * 1024;
    cntl::MemoryAllocatorInitialize( property );

    Test::Suite suite( "AssocArray test" );

    Test::Case* tc[] = {
        new TCAACtorDtor( "Ctor/Dtor" ),
        new TCAAIterator( "Iterator" ),
        new TCAACapacity( "Capacity" ),
        new TCAAModifier( "Modifier" ),
        new TCAAOperation( "Operation" ),
        new TCAAOperator( "Operator" ),
        new TCAAPerformance( "Performance" ),
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
