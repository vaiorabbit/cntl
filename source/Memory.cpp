// -*- coding: utf-8 -*-
#include <cassert>
#include <cstdlib>
#include <cstring>

#include <cntl/Memory.h>

#define Cntl_NameSpace_Begin namespace cntl {
#define Cntl_NameSpace_End   }

Cntl_NameSpace_Begin

MemoryAllocator& AllocatorForSystem()
{
    static MemoryAllocator cAllocatorForSystem;
    return cAllocatorForSystem;
}

MemoryAllocator& AllocatorForSTL()
{
    static MemoryAllocator cAllocatorForSTL;
    return cAllocatorForSTL;
}

bool MemoryAllocatorInitialize( MemoryAllocatorProperty& property )
{
    void* ptr;

    ptr = std::malloc( property.m_uAreaSizeForSystemHeap );
    if ( ptr == NULL )
    {
        return false;
    }
    AllocatorForSystem().Initialize( ptr, property.m_uAreaSizeForSystemHeap );

    ptr = std::malloc( property.m_uAreaSizeForSTLHeap );
    if ( ptr == NULL )
    {
        std::free( AllocatorForSystem().HeapHead() );
        AllocatorForSystem().Finalize();
        return false;
    }
    AllocatorForSTL().Initialize( ptr, property.m_uAreaSizeForSTLHeap );

    return true;
}

void MemoryAllocatorFinalize()
{
    std::free( AllocatorForSystem().HeapHead() );
    AllocatorForSystem().Finalize();

    std::free( AllocatorForSTL().HeapHead() );
    AllocatorForSTL().Finalize();
}

Cntl_NameSpace_End
