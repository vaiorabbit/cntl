// -*- mode: C++; coding: utf-8 -*-
#ifndef TCMEMORYDEBUGALLOCATOR_H_INCLUDED
#define TCMEMORYDEBUGALLOCATOR_H_INCLUDED

#include <TestFramework.h>
#include <cntl/Memory.h>
#include <cntl/STLAllocator.h>

class TCMemoryExample : public Test::Case
{
public:
    TCMemoryExample( const char* name ) : Test::Case( name ) {}

    virtual void Run()
        {
            cntl::MemoryAllocator& ma = cntl::AllocatorForSystem();

            // Allocate/Free
            if ( 1 )
            {
                void* mem_allocated[4];
                mem_allocated[0] = ma.Allocate( 1 * 1024 * 1024 );
                mem_allocated[1] = ma.Allocate( 2 * 1024 * 1024 );
                mem_allocated[2] = ma.Allocate( 3 * 1024 * 1024 );
                mem_allocated[3] = ma.Allocate( 4 * 1024 * 1024 );

                // ma.Dump();

                ma.Free( mem_allocated[3] );
                ma.Free( mem_allocated[1] );
                ma.Free( mem_allocated[0] );
                ma.Free( mem_allocated[2] );

                ma.Dump();
            }

            // AllocateAligned/Free
            if ( 1 )
            {
                void* mem_allocated[4];
                mem_allocated[0] = ma.AllocateAligned( 1 * 1024 * 1024, 16 );
                mem_allocated[1] = ma.AllocateAligned( 2 * 1024 * 1024, 32 );
                mem_allocated[2] = ma.AllocateAligned( 3 * 1024 * 1024, 64 );
                mem_allocated[3] = ma.AllocateAligned( 4 * 1024 * 1024, 128 );

                ma.Dump();

                TEST_ASSERT( (std::size_t)mem_allocated[0] % 16 == 0 );
                TEST_ASSERT( (std::size_t)mem_allocated[1] % 32 == 0 );
                TEST_ASSERT( (std::size_t)mem_allocated[2] % 64 == 0 );
                TEST_ASSERT( (std::size_t)mem_allocated[3] % 128 == 0 );

                ma.Free( mem_allocated[3] );
                ma.Free( mem_allocated[1] );
                ma.Free( mem_allocated[0] );
                ma.Free( mem_allocated[2] );

                ma.Dump();
            }

            // AllocateTail/Free
            if ( 1 )
            {
                void* mem_allocated[4];
                mem_allocated[0] = ma.AllocateTail( 1 * 1024 * 1024 );
                mem_allocated[1] = ma.AllocateTail( 2 * 1024 * 1024 );
                mem_allocated[2] = ma.AllocateTail( 3 * 1024 * 1024 );
                mem_allocated[3] = ma.AllocateTail( 4 * 1024 * 1024 );

                ma.Dump();

                ma.Free( mem_allocated[3] );
                ma.Free( mem_allocated[1] );
                ma.Free( mem_allocated[0] );
                ma.Free( mem_allocated[2] );

                ma.Dump();
            }

            // AllocateAlignedTail/Free
            if ( 1 )
            {
                void* mem_allocated[4];
                mem_allocated[0] = ma.AllocateAlignedTail( 1 * 1024 * 1024, 16 );
                mem_allocated[1] = ma.AllocateAlignedTail( 2 * 1024 * 1024, 32 );
                mem_allocated[2] = ma.AllocateAlignedTail( 3 * 1024 * 1024, 64 );
                mem_allocated[3] = ma.AllocateAlignedTail( 4 * 1024 * 1024, 128 );

                ma.Dump();

                TEST_ASSERT( (std::size_t)mem_allocated[0] % 16 == 0 );
                TEST_ASSERT( (std::size_t)mem_allocated[1] % 32 == 0 );
                TEST_ASSERT( (std::size_t)mem_allocated[2] % 64 == 0 );
                TEST_ASSERT( (std::size_t)mem_allocated[3] % 128 == 0 );

                ma.Free( mem_allocated[3] );
                ma.Free( mem_allocated[1] );
                ma.Free( mem_allocated[0] );
                ma.Free( mem_allocated[2] );

                ma.Dump();
            }

            // ReallocateAlignedTail/Free
            if ( 1 )
            {
                void* mem_allocated[4];
                mem_allocated[0] = ma.Reallocate( NULL, 1 * 1024 * 1024 );
                mem_allocated[1] = ma.Reallocate( NULL, 2 * 1024 * 1024 );
                mem_allocated[2] = ma.Reallocate( NULL, 3 * 1024 * 1024 );
                mem_allocated[3] = ma.Reallocate( NULL, 4 * 1024 * 1024 );

                ma.Dump();

                mem_allocated[0] = ma.Reallocate( mem_allocated[0], 2 * 1024 * 1024 );
                mem_allocated[1] = ma.Reallocate( mem_allocated[1], 2 * 1024 * 1024 );
                mem_allocated[2] = ma.Reallocate( mem_allocated[2], 2 * 1024 * 1024 );
                mem_allocated[3] = ma.Reallocate( mem_allocated[3], 2 * 1024 * 1024 );

                ma.Dump();

                mem_allocated[0] = ma.Reallocate( mem_allocated[0], 4 * 1024 * 1024 );
                mem_allocated[1] = ma.Reallocate( mem_allocated[1], 3 * 1024 * 1024 );
                mem_allocated[2] = ma.Reallocate( mem_allocated[2], 2 * 1024 * 1024 );
                mem_allocated[3] = ma.Reallocate( mem_allocated[3], 1 * 1024 * 1024 );

                ma.Dump();

                ma.Reallocate( mem_allocated[3], 0 );
                ma.Reallocate( mem_allocated[1], 0 );
                ma.Reallocate( mem_allocated[0], 0 );
                ma.Reallocate( mem_allocated[2], 0 );

                ma.Dump();
            }
        }
};

class TCAllocator : public Test::Case
{
public:
    TCAllocator( const char* name ) : Test::Case( name ) {}

    struct Vector3
    {
        Vector3( double x_, double y_, double z_ ) : x(x_), y(y_), z(z_) {}
        double x, y, z;
    };

    virtual void Run()
        {
            std::vector<Vector3, cntl::STLAllocator<Vector3, 16> > v;

            for ( int i = 0; i < 10; ++i )
                v.push_back( Vector3(i, i, i) );
        }
};

#endif
