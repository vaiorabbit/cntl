// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef MEMORYALLOCATOR_H_INCLUDED
#define MEMORYALLOCATOR_H_INCLUDED

#include "EmbList.h"
#include "Mutex.h"

namespace cntl
{
    class MemoryAllocator
    {
    public:

        enum
        {
            Mark_Default = 0
        };

        struct MCB
        {
            EmbListLink used_link;
            EmbListLink free_link;
            char mark;
            char flags;
            char padding[2];
#ifdef DEBUG
            const char* filename;
            std::size_t line;
#endif // DEBUG

            enum
            {
                Flag_None = 0x00U,
                Flag_Used = 0x01U
            };

            void Reset( char mark, char flags )
                {
                    this->mark  = mark;
                    this->flags = flags;
                }

            void SetFlagOn( unsigned char flag )
                { this->flags |= flag; }

            void SetFlagOff( unsigned char flag )
                { this->flags &= ~flag; }

            bool IsFlagOn( unsigned char flag )
                { return (this->flags & flag) != 0; }
        };

        MemoryAllocator();
        ~MemoryAllocator();

        void Initialize( void* heap_head, std::size_t heap_size );
        void Finalize();
        bool Initialized();

        void SetMark( char mark )
            { this->mark = mark; }
        char GetMark()
            { return mark; }

        void* Allocate( std::size_t bytes );
        void* AllocateTail( std::size_t bytes );
        void* AllocateAligned( std::size_t bytes, std::size_t align );
        void* AllocateAlignedTail( std::size_t bytes, std::size_t align );
        void  Free( void* ptr );
        void* Reallocate( void* ptr, std::size_t bytes );

        void* HeapHead()
            { return heap_head; }
        std::size_t HeapSize()
            { return heap_size; }

        void Dump();


        // Default Allocator
        static MemoryAllocator& GetDefault();
        static void SetDefault( MemoryAllocator* default_allocator );

    private:

        MemoryAllocator( const MemoryAllocator& other ); // = delete;
        MemoryAllocator& operator =( const MemoryAllocator& other ); // = delete;

        std::size_t MCBCapacity( MCB* mcb );

        void* AllocateImpl( std::size_t bytes );
        void  FreeImpl( void* ptr );

        void* heap_head;
        std::size_t heap_size;

        char mark;

        typedef EmbList<MCB, offsetof(MCB,used_link)> UsedList;
        typedef EmbList<MCB, offsetof(MCB,free_link)> FreeList;
        UsedList used_list;
        FreeList free_list;

        Mutex mutex;
    };
}

#endif
