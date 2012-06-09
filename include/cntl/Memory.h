// -*- mode: C++; coding: utf-8 -*-
#ifndef CNTL_MEMORY_H_INCLUDED
#define CNTL_MEMORY_H_INCLUDED

#include <cstddef>
#include "MemoryAllocator.h"

namespace cntl
{
    MemoryAllocator& AllocatorForSystem();
    MemoryAllocator& AllocatorForSTL();

    struct MemoryAllocatorProperty
    {
        std::size_t m_uAreaSizeForSystemHeap;
        std::size_t m_uAreaSizeForSTLHeap;
    };

    bool MemoryAllocatorInitialize( MemoryAllocatorProperty& property );
    void MemoryAllocatorFinalize();
}

#define cntlMallocSystem( bytes ) cntl::AllocatorForSystem().Allocate( (bytes) )
#define cntlMallocTailSystem( bytes ) cntl::AllocatorForSystem().Allocate( (bytes) )
#define cntlMallocAlignedSystem( bytes, align ) cntl::AllocatorForSystem().AllocateAligned( (bytes), (align) )
#define cntlMallocAlignedTailSystem( bytes, align ) cntl::AllocatorForSystem().AllocateAligned( (bytes), (align) )
#define cntlFreeSystem( ptr ) cntl::AllocatorForSystem().Free( (ptr) )
#define cntlFreeAlignedSystem( ptr ) cntl::AllocatorForSystem().Free( (ptr) )
#define cntlReallocSystem( ptr, bytes ) cntl::AllocatorForSystem().Reallocate( (ptr), (bytes) )

#define cntlMallocSTL( bytes ) cntl::AllocatorForSTL().Allocate( (bytes) )
#define cntlMallocTailSTL( bytes ) cntl::AllocatorForSTL().Allocate( (bytes) )
#define cntlMallocAlignedSTL( bytes, align ) cntl::AllocatorForSTL().AllocateAligned( (bytes), (align) )
#define cntlMallocAlignedTailSTL( bytes, align ) cntl::AllocatorForSTL().AllocateAligned( (bytes), (align) )
#define cntlFreeSTL( ptr ) cntl::AllocatorForSTL().Free( (ptr) )
#define cntlFreeAlignedSTL( ptr ) cntl::AllocatorForSTL().Free( (ptr) )
#define cntlReallocSTL( ptr, bytes ) cntl::AllocatorForSTL().Reallocate( (ptr), (bytes) )

#endif
