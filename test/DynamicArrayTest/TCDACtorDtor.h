// -*- mode: C++; coding: utf-8 -*-
#ifndef TCDACTORDTOR_H_INCLUDED
#define TCDACTORDTOR_H_INCLUDED

#include <cstdlib>
#include <cntl/DynamicArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCDACtorDtor : public Test::Case
{
    const float tolerance;

public:
    TCDACtorDtor( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            //
            // Make sure lines below causes compiler error C2719.
            // Ref: http://msdn.microsoft.com/en-us/library/373ak2y1(VS.90).aspx
            //
            // std::vector<Vector3> sv_vec3_0;
            // std::vector<Vector3, cntl::Allocator<Vector3,16> > sv_vec3_1;
            //

            cntl::DynamicArray<Vector3> da_vec3_default;
            TEST_ASSERT( da_vec3_default.size() == 0 );

            cntl::DynamicArray<Vector3> da_vec3_sized( 10 );
            TEST_ASSERT( da_vec3_sized.size() == 10 );

            // Scatter one element
            cntl::DynamicArray<Vector3> da_vec3_element( 5, Vector3(1.0f, 2.0f, 3.0f) );
            TEST_ASSERT( da_vec3_element.size() == 5 );
            for ( int i = 0; i < 5; ++i )
            {
                TEST_ASSERT_DOUBLES_EQUAL( da_vec3_element[i].x, 1.0f, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( da_vec3_element[i].y, 2.0f, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( da_vec3_element[i].z, 3.0f, tolerance );
            }

            // From array
            Vector3 vec_array[] = { Vector3(0,0,0), Vector3(1,1,1), Vector3(2,2,2) };
            cntl::DynamicArray<Vector3> da_vec3_from_iterator( vec_array, vec_array+3 );
            TEST_ASSERT( da_vec3_from_iterator.size() == 3 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_from_iterator[0].x, 0.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_from_iterator[0].y, 0.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_from_iterator[0].z, 0.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_from_iterator[1].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_from_iterator[1].y, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_from_iterator[1].z, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_from_iterator[2].x, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_from_iterator[2].y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_from_iterator[2].z, 2.0f, tolerance );

            // Copy constructor
            cntl::DynamicArray<Vector3> da_vec3_copied( da_vec3_from_iterator );
            TEST_ASSERT( da_vec3_copied.size() == 3 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_copied[0].x, 0.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_copied[0].y, 0.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_copied[0].z, 0.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_copied[1].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_copied[1].y, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_copied[1].z, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_copied[2].x, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_copied[2].y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_copied[2].z, 2.0f, tolerance );

            // new/delete
            cntl::DynamicArray<Vector3>* da_vec3_systemheap = new cntl::DynamicArray<Vector3>( 5, Vector3(1.0f, 2.0f, 3.0f) );
            TEST_ASSERT( da_vec3_systemheap->size() == 5 );
            for ( int i = 0; i < 5; ++i )
            {
                TEST_ASSERT_DOUBLES_EQUAL( (*da_vec3_systemheap)[i].x, 1.0f, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( (*da_vec3_systemheap)[i].y, 2.0f, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( (*da_vec3_systemheap)[i].z, 3.0f, tolerance );
            }
            delete da_vec3_systemheap;

            // Placement new/delete
            void* pMemory = std::malloc( sizeof(cntl::DynamicArray<Vector3>) );
            cntl::DynamicArray<Vector3>* da_vec3_mallocheap = ::new ( pMemory ) cntl::DynamicArray<Vector3>( 5, Vector3(1.0f, 2.0f, 3.0f) );
            TEST_ASSERT( da_vec3_mallocheap->size() == 5 );
            for ( int i = 0; i < 5; ++i )
            {
                TEST_ASSERT_DOUBLES_EQUAL( (*da_vec3_mallocheap)[i].x, 1.0f, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( (*da_vec3_mallocheap)[i].y, 2.0f, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( (*da_vec3_mallocheap)[i].z, 3.0f, tolerance );
            }
            da_vec3_mallocheap->cntl::DynamicArray<Vector3>::~DynamicArray(); // Explicit destructor call
            std::free( pMemory );

            //
            // operator =
            //

            // Make sure nothing is modified.
            {
                cntl::DynamicArray<Vector3> da_vec3_assigned( 10 );
                da_vec3_assigned = da_vec3_assigned;
            }

            // source size == 0 : same as calling clear().
            {
                cntl::DynamicArray<Vector3> da_vec3_assigned( 10 );
                cntl::DynamicArray<Vector3> da_vec3_source;
                TEST_ASSERT_EQUAL( da_vec3_source.size(), 0 );
                da_vec3_assigned = da_vec3_source;
                TEST_ASSERT( da_vec3_assigned.size() == 0 );
            }

            // source size <= dest size : erases excess elements in dest array.
            {
                cntl::DynamicArray<Vector3> da_vec3_assigned( 10 );
                cntl::DynamicArray<Vector3> da_vec3_source( 5, Vector3(1,2,3) );
                cntl::DynamicArray<Vector3>::iterator iter_da_start = da_vec3_assigned.begin();
                TEST_ASSERT( da_vec3_source.size() <= da_vec3_assigned.size() );
                da_vec3_assigned = da_vec3_source;
                TEST_ASSERT( da_vec3_assigned.size() == 5 );
                TEST_ASSERT( iter_da_start == da_vec3_assigned.begin() );
            }

            // source size <= dest capacity : contents of dest array are overwritten.
            {
                cntl::DynamicArray<Vector3> da_vec3_assigned( 1 );
                cntl::DynamicArray<Vector3> da_vec3_source( 5, Vector3(1,2,3) );
                da_vec3_assigned.reserve( 10 );
                TEST_ASSERT( da_vec3_source.size() > da_vec3_assigned.size() );
                TEST_ASSERT( da_vec3_source.size() <= da_vec3_assigned.capacity() );
                cntl::DynamicArray<Vector3>::iterator iter_da_start = da_vec3_assigned.begin();
                da_vec3_assigned = da_vec3_source;
                TEST_ASSERT( da_vec3_assigned.size() == 5 );
                TEST_ASSERT( da_vec3_assigned.capacity() == 10 );
                TEST_ASSERT( iter_da_start == da_vec3_assigned.begin() );
            }

            // There's no sufficient capacity in dest array : contents of source array are copied to newly allocated area.
            {
                cntl::DynamicArray<Vector3> da_vec3_assigned( 5 );
                cntl::DynamicArray<Vector3> da_vec3_source( 10, Vector3(1,2,3) );
                TEST_ASSERT( da_vec3_source.size() > da_vec3_assigned.size() );
                TEST_ASSERT( da_vec3_source.size() > da_vec3_assigned.capacity() );
                cntl::DynamicArray<Vector3>::iterator iter_da_start = da_vec3_assigned.begin();
                da_vec3_assigned = da_vec3_source;
                TEST_ASSERT( da_vec3_assigned.size() == 10 );
                TEST_ASSERT( da_vec3_assigned.capacity() == 10 );
                TEST_ASSERT( iter_da_start != da_vec3_assigned.begin() );
            }
        }
};

#endif
