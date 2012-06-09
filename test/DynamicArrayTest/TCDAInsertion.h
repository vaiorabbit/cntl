// -*- mode: C++; coding: utf-8 -*-
#ifndef TCDAINSERTION_H_INCLUDED
#define TCDAINSERTION_H_INCLUDED

#include <cntl/Memory.h>
#include <cntl/DynamicArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCDAInsertion : public Test::Case
{
    const float tolerance;

public:
    TCDAInsertion( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // Inserts a single element at pos.
            // iterator insert( iterator pos, const value_type& val )

            // no sufficient capacity
            cntl::DynamicArray<Vector3> da_vec3_insert_one( 9, Vector3(0,0,0) );
            cntl::AllocatorForSTL().Dump();

            da_vec3_insert_one.insert( da_vec3_insert_one.begin()+4, Vector3(4,4,4) );
            TEST_ASSERT( da_vec3_insert_one.size() == 10 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[4].x, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[4].y, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[4].z, 4.0f, tolerance );
            cntl::AllocatorForSTL().Dump();

            da_vec3_insert_one.insert( da_vec3_insert_one.end(), Vector3(10,10,10) );
            TEST_ASSERT( da_vec3_insert_one.size() == 11 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[10].x, 10.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[10].y, 10.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[10].z, 10.0f, tolerance );
            cntl::AllocatorForSTL().Dump();

            da_vec3_insert_one.insert( da_vec3_insert_one.begin(), Vector3(11,11,11) );
            TEST_ASSERT( da_vec3_insert_one.size() == 12 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[0].x, 11.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[0].y, 11.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[0].z, 11.0f, tolerance );
            cntl::AllocatorForSTL().Dump();

            // provide sufficient capacity in advance
            cntl::DynamicArray<Vector3>().swap( da_vec3_insert_one );
            TEST_ASSERT( da_vec3_insert_one.capacity() == 0 );
            da_vec3_insert_one.reserve( 50 );
            da_vec3_insert_one.resize( 9, Vector3(0,0,0) );

            da_vec3_insert_one.insert( da_vec3_insert_one.begin()+4, Vector3(4,4,4) );
            TEST_ASSERT( da_vec3_insert_one.size() == 10 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[4].x, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[4].y, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[4].z, 4.0f, tolerance );

            da_vec3_insert_one.insert( da_vec3_insert_one.end(), Vector3(10,10,10) );
            TEST_ASSERT( da_vec3_insert_one.size() == 11 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[10].x, 10.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[10].y, 10.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[10].z, 10.0f, tolerance );

            da_vec3_insert_one.insert( da_vec3_insert_one.begin(), Vector3(11,11,11) );
            TEST_ASSERT( da_vec3_insert_one.size() == 12 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[0].x, 11.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[0].y, 11.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_one[0].z, 11.0f, tolerance );


            // Inserts elements at pos.
            // void insert( iterator pos, size_type count, const value_type& val )
            // no sufficient capacity
            cntl::DynamicArray<Vector3> da_vec3_insert_multiple( 9, Vector3(0,0,0) );
            da_vec3_insert_multiple.insert( da_vec3_insert_multiple.begin()+4, 3, Vector3(4,4,4) );
            TEST_ASSERT( da_vec3_insert_multiple.size() == 12 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[4].x, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[4].y, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[4].z, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[5].x, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[5].y, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[5].z, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[6].x, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[6].y, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[6].z, 4.0f, tolerance );

            da_vec3_insert_multiple.insert( da_vec3_insert_multiple.end(), 3, Vector3(14,14,14) );
            TEST_ASSERT( da_vec3_insert_multiple.size() == 15 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[12].x, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[12].y, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[12].z, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[13].x, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[13].y, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[13].z, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[14].x, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[14].y, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[14].z, 14.0f, tolerance );

            da_vec3_insert_multiple.insert( da_vec3_insert_multiple.begin(), 3, Vector3(17,17,17) );
            TEST_ASSERT( da_vec3_insert_multiple.size() == 18 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[0].x, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[0].y, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[0].z, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[1].x, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[1].y, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[1].z, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[2].x, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[2].y, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[2].z, 17.0f, tolerance );

            // provide sufficient capacity in advance
            cntl::DynamicArray<Vector3>().swap( da_vec3_insert_multiple );
            TEST_ASSERT( da_vec3_insert_multiple.capacity() == 0 );
            da_vec3_insert_multiple.reserve( 50 );
            da_vec3_insert_multiple.resize( 9, Vector3(0,0,0) );

            da_vec3_insert_multiple.insert( da_vec3_insert_multiple.begin()+4, 3, Vector3(4,4,4) );
            TEST_ASSERT( da_vec3_insert_multiple.size() == 12 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[4].x, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[4].y, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[4].z, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[5].x, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[5].y, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[5].z, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[6].x, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[6].y, 4.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[6].z, 4.0f, tolerance );

            da_vec3_insert_multiple.insert( da_vec3_insert_multiple.end(), 3, Vector3(14,14,14) );
            TEST_ASSERT( da_vec3_insert_multiple.size() == 15 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[12].x, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[12].y, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[12].z, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[13].x, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[13].y, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[13].z, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[14].x, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[14].y, 14.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[14].z, 14.0f, tolerance );

            da_vec3_insert_multiple.insert( da_vec3_insert_multiple.begin(), 3, Vector3(17,17,17) );
            TEST_ASSERT( da_vec3_insert_multiple.size() == 18 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[0].x, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[0].y, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[0].z, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[1].x, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[1].y, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[1].z, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[2].x, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[2].y, 17.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_multiple[2].z, 17.0f, tolerance );


            // Inserts a range of elements [first, last).
            // void insert( iterator pos, InputIterator first, InputIterator last )
            // no sufficient capacity
            Vector3 vec_array[] = { Vector3(1,1,1), Vector3(2,2,2), Vector3(3,3,3) };
            cntl::DynamicArray<Vector3> da_vec3_insert_range( 9, Vector3(0,0,0) );
            da_vec3_insert_range.insert( da_vec3_insert_range.begin()+4, vec_array, vec_array+3 );
            TEST_ASSERT( da_vec3_insert_range.size() == 12 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[4].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[4].y, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[4].z, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[5].x, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[5].y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[5].z, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[6].x, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[6].y, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[6].z, 3.0f, tolerance );

            da_vec3_insert_range.insert( da_vec3_insert_range.end(), vec_array, vec_array+3 );
            TEST_ASSERT( da_vec3_insert_range.size() == 15 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[12].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[12].y, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[12].z, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[13].x, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[13].y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[13].z, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[14].x, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[14].y, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[14].z, 3.0f, tolerance );

            da_vec3_insert_range.insert( da_vec3_insert_range.begin(), vec_array, vec_array+3 );
            TEST_ASSERT( da_vec3_insert_range.size() == 18 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[0].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[0].y, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[0].z, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[1].x, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[1].y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[1].z, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[2].x, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[2].y, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[2].z, 3.0f, tolerance );

            // provide sufficient capacity in advance
            cntl::DynamicArray<Vector3>().swap( da_vec3_insert_range );
            TEST_ASSERT( da_vec3_insert_range.capacity() == 0 );
            da_vec3_insert_range.reserve( 50 );
            da_vec3_insert_range.resize( 9, Vector3(0,0,0) );

            da_vec3_insert_range.insert( da_vec3_insert_range.begin()+4, vec_array, vec_array+3 );
            TEST_ASSERT( da_vec3_insert_range.size() == 12 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[4].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[4].y, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[4].z, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[5].x, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[5].y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[5].z, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[6].x, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[6].y, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[6].z, 3.0f, tolerance );

            da_vec3_insert_range.insert( da_vec3_insert_range.end(), vec_array, vec_array+3 );
            TEST_ASSERT( da_vec3_insert_range.size() == 15 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[12].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[12].y, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[12].z, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[13].x, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[13].y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[13].z, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[14].x, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[14].y, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[14].z, 3.0f, tolerance );

            da_vec3_insert_range.insert( da_vec3_insert_range.begin(), vec_array, vec_array+3 );
            TEST_ASSERT( da_vec3_insert_range.size() == 18 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[0].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[0].y, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[0].z, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[1].x, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[1].y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[1].z, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[2].x, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[2].y, 3.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_insert_range[2].z, 3.0f, tolerance );


            // push_back
            // no sufficient capacity
            cntl::DynamicArray<Vector3> da_vec3_push( 3, Vector3(0,0,0) );
            da_vec3_push.push_back( Vector3(1,2,3) );
            TEST_ASSERT( da_vec3_push.size() == 4 );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_push[3].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_push[3].y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_push[3].z, 3.0f, tolerance );

            // provide sufficient capacity in advance
            cntl::DynamicArray<Vector3>().swap( da_vec3_push );
            da_vec3_push.reserve( 10 );
            da_vec3_push.resize( 3, Vector3(0,0,0) );
            da_vec3_push.push_back( Vector3(1,2,3) );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_push[3].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_push[3].y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_push[3].z, 3.0f, tolerance );

            // pop_back

            cntl::DynamicArray<Vector3> da_vec3_pop( 3, Vector3(0,0,0) );
            da_vec3_pop.pop_back();
            TEST_ASSERT( da_vec3_pop.size() == 2 );
            da_vec3_pop.pop_back();
            da_vec3_pop.pop_back();
            TEST_ASSERT( da_vec3_pop.size() == 0 );
            da_vec3_pop.pop_back();
            TEST_ASSERT( da_vec3_pop.size() == 0 );

            // Replaces all the elements with +count+ times of copy of +val+.
            // void assign( size_type count, const Type& val )
            cntl::DynamicArray<Vector3> da_vec3_assign( 10, Vector3(0,0,0) );
            da_vec3_assign.assign( 3, Vector3(1,1,1) );
            TEST_ASSERT( da_vec3_assign.size() == 3 );
            TEST_ASSERT( da_vec3_assign.capacity() == 10 );
            for ( int i = 0; i < 3; ++i )
            {
                TEST_ASSERT_DOUBLES_EQUAL( da_vec3_assign[i].x, 1.0f, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( da_vec3_assign[i].y, 1.0f, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( da_vec3_assign[i].z, 1.0f, tolerance );
            }

            // Replaces all the elements with elements in the range [first, last).
            // void assign( InputIterator first, InputIterator last )
            cntl::DynamicArray<Vector3> da_vec3_assign_range( 10, Vector3(0,0,0) );
            da_vec3_assign_range.assign( da_vec3_assign.begin(), da_vec3_assign.end() );
            TEST_ASSERT( da_vec3_assign_range.size() == 3 );
            TEST_ASSERT( da_vec3_assign_range.capacity() == 10 );
            for ( int i = 0; i < 3; ++i )
            {
                TEST_ASSERT_DOUBLES_EQUAL( da_vec3_assign_range[i].x, 1.0f, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( da_vec3_assign_range[i].y, 1.0f, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( da_vec3_assign_range[i].z, 1.0f, tolerance );
            }
        }
};

#endif
