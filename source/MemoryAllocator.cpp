// -*- mode: C++; coding: utf-8-dos; -*-
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cntl/MemoryAllocator.h>

#define ZEROFREEBLOCK_TEST

#define Cntl_NameSpace_Begin namespace cntl {
#define Cntl_NameSpace_End   }

Cntl_NameSpace_Begin

static inline std::size_t MaxOf( std::size_t a, std::size_t b )
{
    return (a < b) ? b : a;
}

const int MCB_Align_Default = 16;

static MemoryAllocator* s_default_allocator = NULL;

// Ref.: http://stackoverflow.com/questions/1010922/question-about-round-up-macro
#define ROUNDUP( value, align ) \
    static_cast<std::size_t>( ((value)+((align)-1)) & (~((align)-1)) )

#define ROUNDDOWN( value, align ) \
    static_cast<std::size_t>( ((value)) & (~((align)-1)) )

std::size_t MemoryAllocator::MCBCapacity( MCB* mcb )
{
    MCB* mcb_next = used_list.GetNext( mcb->used_link );
    if ( mcb_next )
    {
        return (std::size_t)(mcb_next)-(std::size_t)(mcb)-sizeof(MCB);
    }
    else
    {
        return (std::size_t)(heap_head)+heap_size-(std::size_t)(mcb)-sizeof(MCB);
    }
}

MemoryAllocator::MemoryAllocator()
    : heap_head(NULL)
    , heap_size(0)
    , mark(Mark_Default)
    , mutex()
{}

MemoryAllocator::~MemoryAllocator()
{}


void MemoryAllocator::Initialize( void* heap_head, std::size_t heap_size )
{
    this->heap_head = heap_head;
    this->heap_size = heap_size;

    std::memset( heap_head, 0xcd, heap_size );

    used_list.Initialize();
    free_list.Initialize();

    MCB* mcb_head = static_cast<MCB*>(heap_head);
    mcb_head->Reset( Mark_Default, MCB::Flag_None );
    used_list.PushFront( &mcb_head->used_link );
    free_list.PushFront( &mcb_head->free_link );
}

void MemoryAllocator::Finalize()
{
    heap_head = NULL;
    heap_size = 0;
}

bool MemoryAllocator::Initialized()
{
    return (heap_head == NULL) && (heap_size == 0);
}

void* MemoryAllocator::Allocate( std::size_t bytes )
{
    ScopedLock lock( this->mutex );
    return AllocateImpl( bytes );
}

void* MemoryAllocator::AllocateImpl( std::size_t bytes )
{
    std::size_t bytes_aligned = ROUNDUP( bytes, MCB_Align_Default );

    // search sufficient unused MCB
    MCB* mcb = NULL;
    for ( mcb = free_list.GetHead(); mcb; mcb = free_list.GetNext(mcb->free_link) )
    {
        if ( MCBCapacity(mcb) >= bytes_aligned )
            break;
    }

    if ( mcb == NULL )
        return NULL;


    // divide large MCB if possible
    std::size_t bytes_divisible = bytes_aligned + sizeof(MCB);
    if ( MCBCapacity(mcb) > bytes_divisible )
    {
        MCB* mcb_new = reinterpret_cast<MCB*>( reinterpret_cast<std::size_t>(mcb) + bytes_divisible );
        mcb_new->Reset( Mark_Default, MCB::Flag_None );
        used_list.InsertNext( &mcb->used_link, &mcb_new->used_link );
        free_list.InsertNext( &mcb->free_link, &mcb_new->free_link );
        mcb_new->SetFlagOff( MCB::Flag_Used );
    }

    // unlink MCB from free list
    mcb->SetFlagOn( MCB::Flag_Used );
    free_list.Remove( &mcb->free_link );

    return reinterpret_cast<void*>( reinterpret_cast<std::size_t>(mcb) + sizeof(MCB) );
}

void* MemoryAllocator::AllocateTail( std::size_t bytes )
{
    ScopedLock lock( this->mutex );

    std::size_t bytes_aligned = ROUNDUP( bytes, MCB_Align_Default );

    // search sufficient unused MCB
    MCB* mcb = NULL;
    for ( mcb = free_list.GetTail(); mcb; mcb = free_list.GetPrev(mcb->free_link) )
    {
        if ( MCBCapacity(mcb) >= bytes_aligned )
            break;
    }

    if ( mcb == NULL )
        return NULL;

    // divide large MCB if possible
    std::size_t bytes_divisible = bytes_aligned + sizeof(MCB);
    if ( MCBCapacity(mcb) > bytes_divisible )
    {
        MCB* mcb_new =
            reinterpret_cast<MCB*>(
                reinterpret_cast<std::size_t>(mcb) + sizeof(MCB) + MCBCapacity(mcb)
                - bytes_divisible );
        mcb_new->Reset( Mark_Default, MCB::Flag_None );
        used_list.InsertNext( &mcb->used_link, &mcb_new->used_link );
        free_list.InsertNext( &mcb->free_link, &mcb_new->free_link );

        mcb = mcb_new;
    }

    // unlink MCB from free list
    mcb->SetFlagOn( MCB::Flag_Used );
    free_list.Remove( &mcb->free_link );

    return reinterpret_cast<void*>( reinterpret_cast<std::size_t>(mcb) + sizeof(MCB) );
}

void* MemoryAllocator::AllocateAligned( std::size_t bytes, std::size_t align )
{
    ScopedLock lock( this->mutex );

    std::size_t bytes_aligned = ROUNDUP( bytes, MCB_Align_Default );

    std::size_t bytes_padding = 0; // padding bytes for adjusting return address alignment

    // search sufficient unused MCB
    MCB* mcb = NULL;
    for ( mcb = free_list.GetHead(); mcb; mcb = free_list.GetNext(mcb->free_link) )
    {
        std::size_t addr = reinterpret_cast<std::size_t>(mcb) + sizeof(MCB);
        std::size_t addr_aligned = ROUNDUP( addr, align );
        bytes_padding = addr_aligned - addr;
        assert( bytes_padding >= 0 );

        // needs padding, but #bytes_padding is insufficient to contain MCB...
        if ( bytes_padding && bytes_padding < sizeof(MCB) )
        {
            // then recalculate padding bytes
            addr_aligned = ROUNDUP( reinterpret_cast<std::size_t>(mcb) + 2 * sizeof(MCB), align );
            bytes_padding = addr_aligned - addr;
        }

        if ( MCBCapacity(mcb) >= bytes_aligned + bytes_padding )
            break;
    }

    if ( mcb == NULL )
        return NULL;

    // create new MCB that can return aligned address
    if ( bytes_padding != 0 )
    {
        assert( bytes_padding >= sizeof(MCB) );

        MCB* mcb_aligned = reinterpret_cast<MCB*>( reinterpret_cast<std::size_t>(mcb) + bytes_padding );
        assert( (reinterpret_cast<std::size_t>(mcb_aligned) + sizeof(MCB)) % align == 0 );

        mcb_aligned->Reset( Mark_Default, MCB::Flag_None );
        used_list.InsertNext( &mcb->used_link, &mcb_aligned->used_link );
        free_list.InsertNext( &mcb->free_link, &mcb_aligned->free_link );

        // +mcb_aligned+ may occupy whole capacity of +mcb+.
        if ( MCBCapacity(mcb) == 0 )
        {
            // To eliminate 'dead (unused but capacity==0)' mcb,
            // merge with previous MCB
            MCB* mcb_prev = used_list.GetPrev( mcb->used_link );
            if ( mcb_prev /*&& !mcb_prev->IsFlagOn( MCB::Flag_Used )*/ )
            {
                used_list.MergeAdjacent( &mcb_prev->used_link, &mcb->used_link );
                free_list.Remove( &mcb->free_link );
            }
        }

        mcb = mcb_aligned;
    }

    // divide large MCB if possible
    std::size_t bytes_divisible = bytes_aligned + sizeof(MCB);
    if ( MCBCapacity(mcb) > bytes_divisible )
    {
        MCB* mcb_new = reinterpret_cast<MCB*>( reinterpret_cast<std::size_t>(mcb) + bytes_divisible );
        mcb_new->Reset( Mark_Default, MCB::Flag_None );
        used_list.InsertNext( &mcb->used_link, &mcb_new->used_link );
        free_list.InsertNext( &mcb->free_link, &mcb_new->free_link );
        mcb_new->SetFlagOff( MCB::Flag_Used );
    }

    // unlink MCB from free list
    mcb->SetFlagOn( MCB::Flag_Used );
    free_list.Remove( &mcb->free_link );

    assert( (reinterpret_cast<std::size_t>(mcb) + sizeof(MCB)) % align == 0 );
    return reinterpret_cast<void*>( reinterpret_cast<std::size_t>(mcb) + sizeof(MCB) );
}

void* MemoryAllocator::AllocateAlignedTail( std::size_t bytes, std::size_t align )
{
    ScopedLock lock( this->mutex );

    std::size_t bytes_aligned = ROUNDUP( bytes, MCB_Align_Default );

    std::size_t bytes_padding = 0; // padding bytes for adjusting return address alignment

    // search sufficient unused MCB
    MCB* mcb = NULL;
    for ( mcb = free_list.GetTail(); mcb; mcb = free_list.GetPrev(mcb->free_link) )
    {
        std::size_t addr = 
            reinterpret_cast<std::size_t>(mcb) + sizeof(MCB) + MCBCapacity(mcb)
            - bytes_aligned;
        std::size_t addr_aligned = ROUNDDOWN( addr, align );
        bytes_padding = addr - addr_aligned;
        assert( bytes_padding >= 0 );

        if ( MCBCapacity(mcb) >= sizeof(MCB) + bytes_aligned + bytes_padding )
            break;
    }

    if ( mcb == NULL )
        return NULL;


    // create new MCB that can return aligned address
    // if ( bytes_padding != 0 )
    {
        MCB* mcb_aligned = reinterpret_cast<MCB*>(
            reinterpret_cast<std::size_t>(mcb) + sizeof(MCB) + MCBCapacity(mcb)
            - bytes_aligned - bytes_padding - sizeof(MCB)
            );
        assert( (reinterpret_cast<std::size_t>(mcb_aligned) + sizeof(MCB)) % align == 0 );

        mcb_aligned->Reset( Mark_Default, MCB::Flag_None );
        used_list.InsertNext( &mcb->used_link, &mcb_aligned->used_link );
        free_list.InsertNext( &mcb->free_link, &mcb_aligned->free_link );

        // +mcb_aligned+ may occupy whole capacity of +mcb+.
        if ( MCBCapacity(mcb) == 0 )
        {
            // To eliminate 'dead (unused but capacity==0)' mcb,
            // merge with previous MCB
            MCB* mcb_prev = used_list.GetPrev( mcb->used_link );
            if ( mcb_prev /*&& !mcb_prev->IsFlagOn( MCB::Flag_Used )*/ )
            {
                used_list.MergeAdjacent( &mcb_prev->used_link, &mcb->used_link );
                free_list.Remove( &mcb->free_link );
            }
        }

        mcb = mcb_aligned;
    }

    // unlink MCB from free list
    mcb->SetFlagOn( MCB::Flag_Used );
    free_list.Remove( &mcb->free_link );

    return reinterpret_cast<void*>( reinterpret_cast<std::size_t>(mcb) + sizeof(MCB) );
}

void  MemoryAllocator::Free( void* ptr )
{
    ScopedLock lock( this->mutex );
    FreeImpl( ptr );
}

void  MemoryAllocator::FreeImpl( void* ptr )
{
    if ( ptr == NULL )
        return;
    
    MCB* mcb = static_cast<MCB*>(ptr) - 1;

    assert( mcb->IsFlagOn( MCB::Flag_Used ) );
    if ( !mcb->IsFlagOn( MCB::Flag_Used ) )
        return;

    bool merged = false;

    // merge with previous MCB
    MCB* mcb_prev = used_list.GetPrev( mcb->used_link );
    if ( mcb_prev && !mcb_prev->IsFlagOn( MCB::Flag_Used ) )
    {
        used_list.MergeAdjacent( &mcb_prev->used_link, &mcb->used_link );

        mcb = mcb_prev;
        merged = true;
    }

    // merge with next MCB
    MCB* mcb_next = used_list.GetNext( mcb->used_link );
    if ( mcb_next && !mcb_next->IsFlagOn( MCB::Flag_Used ) )
    {
        if ( free_list.IsAdjacent(&mcb->free_link, &mcb_next->free_link) )
            free_list.MergeAdjacent( &mcb->free_link, &mcb_next->free_link );
        else
            free_list.MergeNotAdjacent( &mcb->free_link, &mcb_next->free_link );

        used_list.MergeAdjacent( &mcb->used_link, &mcb_next->used_link );

        merged = true;
    }

    // put MCB back to free_list
    if ( !merged )
    {
        assert( mcb->IsFlagOn( MCB::Flag_Used ) );

        if ( mcb < free_list.GetHead() )
        {
            free_list.PushFront( &mcb->free_link );
            merged = true;
        }
        else if ( mcb > free_list.GetTail() )
        {
            free_list.PushBack( &mcb->free_link );
            merged = true;
        }
        else
        {
            MCB* mcb_fl = NULL;
            for ( mcb_fl = free_list.GetHead(); mcb_fl; mcb_fl = free_list.GetNext(mcb_fl->free_link) )
            {
                MCB* mcb_fl_next = free_list.GetNext( mcb_fl->free_link );
                if ( mcb_fl < mcb && mcb < mcb_fl_next )
                {
                    free_list.InsertNext( &mcb_fl->free_link, &mcb->free_link );
                    merged = true;
                    break;
                }
            }
        }

        assert( merged );
        if ( !merged ) // for safety
        {
            free_list.PushFront( &mcb->free_link );
        }
    }

    mcb->SetFlagOff( MCB::Flag_Used );
}

void* MemoryAllocator::Reallocate( void* ptr, std::size_t bytes )
{
    ScopedLock lock( this->mutex );

    if ( !ptr )
        return AllocateImpl( bytes );

    if ( bytes == 0 )
    {
        FreeImpl( ptr );
        return NULL;
    }

    MCB* mcb = static_cast<MCB*>(ptr) - 1;
    assert( mcb->IsFlagOn( MCB::Flag_Used ) );

    if ( bytes == MCBCapacity(mcb) )
        return ptr;

    MCB* mcb_next = used_list.GetNext( mcb->used_link );

    // Needs larger space.
    if ( bytes > MCBCapacity(mcb) )
    {
        std::size_t size_to_extend = bytes - MCBCapacity(mcb);

        // Next block is unused, contiguous and sufficient
        if ( mcb_next && !mcb_next->IsFlagOn( MCB::Flag_Used ) && MCBCapacity(mcb_next) > MaxOf(ROUNDUP(size_to_extend, MCB_Align_Default), sizeof(MCB)) )
        {
            MCB* mcb_new = reinterpret_cast<MCB*>( reinterpret_cast<std::size_t>(mcb_next) + ROUNDUP(size_to_extend, MCB_Align_Default) );

            // Backup mcb_next property
            EmbListLink* mcb_next_used_link_prev = mcb_next->used_link.prev;
            EmbListLink* mcb_next_used_link_next = mcb_next->used_link.next;
            EmbListLink* mcb_next_free_link_prev = mcb_next->free_link.prev;
            EmbListLink* mcb_next_free_link_next = mcb_next->free_link.next;

            // Replace mcb_next address with that of mcb_new
            (mcb_next->used_link.prev ? mcb_next->used_link.prev->next : used_list.head) = &mcb_new->used_link;
            (mcb_next->used_link.next ? mcb_next->used_link.next->prev : used_list.tail) = &mcb_new->used_link;
            (mcb_next->free_link.prev ? mcb_next->free_link.prev->next : free_list.head) = &mcb_new->free_link;
            (mcb_next->free_link.next ? mcb_next->free_link.next->prev : free_list.tail) = &mcb_new->free_link;

            // Copy mcb_next property into mcb_new
            mcb_new->used_link.prev = mcb_next_used_link_prev;
            mcb_new->used_link.next = mcb_next_used_link_next;
            mcb_new->free_link.prev = mcb_next_free_link_prev;
            mcb_new->free_link.next = mcb_next_free_link_next;

            mcb_new->Reset( Mark_Default, MCB::Flag_None );
            mcb_new->SetFlagOff( MCB::Flag_Used );

            return ptr;
        }
        else
        {
            void* large_area = AllocateImpl( bytes );
            assert( large_area );

            if ( large_area )
            {
                std::memcpy( large_area, ptr, MCBCapacity(mcb) );
                FreeImpl( ptr );
            }

            return large_area;
        }
    }
    // Shrink current space.
    else // bytes < MCBCapacity(mcb)
    {
        std::size_t size_to_shrink = MCBCapacity(mcb) - bytes;

        // Create new free block.
        // [NOTE] Do nothing if the size of redundant area is not sufficient to contain one MCB
        if ( size_to_shrink > sizeof(MCB) )
        {
            MCB* mcb_new = reinterpret_cast<MCB*>( reinterpret_cast<std::size_t>(mcb_next) - size_to_shrink );
            mcb_new->Reset( Mark_Default, MCB::Flag_None );
            used_list.InsertNext( &mcb->used_link, &mcb_new->used_link );
            mcb_new->SetFlagOff( MCB::Flag_Used );

            // Search proper position
            if ( mcb_new < free_list.GetHead() )
            {
                free_list.PushFront( &mcb_new->free_link );
            }
            else if ( mcb_new > free_list.GetTail() )
            {
                free_list.PushBack( &mcb_new->free_link );
            }
            else
            {
                MCB* mcb_fl = NULL;
                for ( mcb_fl = free_list.GetHead(); mcb_fl; mcb_fl = free_list.GetNext(mcb_fl->free_link) )
                {
                    MCB* mcb_fl_next = free_list.GetNext( mcb_fl->free_link );
                    if ( mcb_fl < mcb_new && mcb_new < mcb_fl_next )
                    {
                        free_list.InsertNext( &mcb_fl->free_link, &mcb_new->free_link );
                        break;
                    }
                }
            }

            // Merge new MCB if mcb_next is unused
            if ( mcb_next && !mcb_next->IsFlagOn( MCB::Flag_Used ) )
            {
                assert( free_list.GetNext(mcb_new->free_link) == mcb_next );
                used_list.MergeAdjacent( &mcb_new->used_link, &mcb_next->used_link );
                free_list.MergeAdjacent( &mcb_new->free_link, &mcb_next->free_link );
            }
        }

        return ptr;
    }
}


void MemoryAllocator::Dump()
{
    ScopedLock lock( this->mutex );

    std::printf( "\n== MemoryAllocator (%p)==\n", this );
    std::printf( "heap_head=%p, heap_size=%lu, sizeof(MCB)=%lu\n", this->heap_head, this->heap_size, sizeof(MCB) );

    std::size_t mcb_count = 0;
    std::size_t mcb_size_sum = 0;
    std::printf( "\n=== Used List ===\n" );
    mcb_count = 0;
    mcb_size_sum = 0;
    for ( MCB* mcb = used_list.GetHead(); mcb; mcb = used_list.GetNext(mcb->used_link) )
    {
        std::printf( "ID%05lu (%p): Flags=%02x, Size=%8lu\n", mcb_count, mcb, mcb->flags, MCBCapacity(mcb) );
        ++mcb_count;
        mcb_size_sum += MCBCapacity(mcb);
    }
    std::printf( "mcb_size_sum=%lu, MCB bytes=%lu total=%lu\n", mcb_size_sum, mcb_count*sizeof(MCB), mcb_size_sum+mcb_count*sizeof(MCB) );

    std::size_t free_size_sum = 0;
    std::size_t free_size_max = 0;
    std::printf( "\n=== Free List ===\n" );
    mcb_count = 0;
    for ( MCB* mcb = free_list.GetHead(); mcb; mcb = free_list.GetNext(mcb->free_link) )
    {
        std::printf( "ID%05lu (%p): Flags=%02x, Size=%8lu\n", mcb_count, mcb, mcb->flags, MCBCapacity(mcb) );
        ++mcb_count;
        free_size_sum += MCBCapacity(mcb);
        if ( free_size_max < MCBCapacity(mcb) )
            free_size_max = MCBCapacity(mcb);
    }
    std::printf( "free_size_sum=%lu, free_size_max=%lu\n", free_size_sum, free_size_max );
}

std::size_t MemoryAllocator::GetMaxFreeAreaSize()
{
    ScopedLock lock( this->mutex );

    std::size_t free_size_max = 0;
    mcb_count = 0;
    for ( MCB* mcb = free_list.GetHead(); mcb; mcb = free_list.GetNext(mcb->free_link) )
    {
        if ( free_size_max < MCBCapacity(mcb) )
            free_size_max = MCBCapacity(mcb);
    }

    return free_size_max;
}

std::size_t MemoryAllocator::GetFreeSizeSum()
{
    ScopedLock lock( this->mutex );

    std::size_t free_size_sum = 0;
    for ( MCB* mcb = free_list.GetHead(); mcb; mcb = free_list.GetNext(mcb->free_link) )
    {
        free_size_sum += MCBCapacity(mcb);
    }

    return free_size_sum;
}


// static
MemoryAllocator& MemoryAllocator::GetDefault()
{
    return *s_default_allocator;
}

// static
void MemoryAllocator::SetDefault( MemoryAllocator* default_allocator )
{
    s_default_allocator = default_allocator;
}

Cntl_NameSpace_End
