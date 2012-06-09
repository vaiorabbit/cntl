// -*- mode: C++; coding: utf-8 -*-
#include <TestFramework.h>

#include "TCDACtorDtor.h"
#include "TCDAResizeReserve.h"
#include "TCDADeletion.h"
#include "TCDAInsertion.h"
#include "TCDAIterator.h"
#include "TCDAAccessor.h"
#include "TCDAOperation.h"
#include "TCDAAlgorithm.h"

int
main( int argc, char** argv )
{
    cntl::MemoryAllocatorProperty property;
    property.m_uAreaSizeForSystemHeap = 8 * 1024 * 1024;
    property.m_uAreaSizeForSTLHeap = 8 * 1024 * 1024;
    cntl::MemoryAllocatorInitialize( property );

    Test::Suite suite( "DynamicArray test" );

    Test::Case* tc[] = {
        new TCDACtorDtor( "Ctor/Dtor" ),
        new TCDAResizeReserve( "Resize/Reserve" ),
        new TCDADeletion( "Delete operation" ),
        new TCDAInsertion( "Insert / Push / Pop" ),
        new TCDAIterator( "Iterator" ),
        new TCDAAccessor( "Accessor" ),
        new TCDAOperation( "Operation" ),
        new TCDAAlgorithm( "Algorithm" ),
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
