// -*- mode: C++; coding: utf-8 -*-
#ifndef TCDAOPERATION_H_INCLUDED
#define TCDAOPERATION_H_INCLUDED

#include <cntl/DynamicArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCDAOperation : public Test::Case
{
    const float tolerance;

public:
    TCDAOperation( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // swap
            cntl::DynamicArray<Vector3> da_vec3_swap0( 5, Vector3(1,1,1) );
            cntl::DynamicArray<Vector3> da_vec3_swap1( 10, Vector3(2,2,2) );
            da_vec3_swap0.swap( da_vec3_swap1 );
            TEST_ASSERT_EQUAL( da_vec3_swap0.size(), 10 );
            TEST_ASSERT_EQUAL( da_vec3_swap1.size(), 5 );

            // get_allocator
            cntl::DynamicArray<Vector3> da_vec3_alloc;
            cntl::DynamicArray<Vector3>::allocator_type ma = da_vec3_alloc.get_allocator();
            Vector3* area = (Vector3*)ma.allocate( 65536 );
            TEST_ASSERT( area != NULL );
            ma.deallocate( area, 65536 );

            // operator ==, !=
            cntl::DynamicArray<int> da_int_eq_v0, da_int_eq_v1;
            da_int_eq_v0.reserve( 5 );
            for ( unsigned i = 0; i < da_int_eq_v0.capacity(); ++i )
                da_int_eq_v0.push_back( i );

            da_int_eq_v1.reserve( 5 );
            for ( unsigned i = 0; i < da_int_eq_v1.capacity(); ++i )
                da_int_eq_v1.push_back( i );

            TEST_ASSERT_EQUAL( da_int_eq_v0 == da_int_eq_v1, true );
            TEST_ASSERT_EQUAL( da_int_eq_v0 != da_int_eq_v1, false );

            da_int_eq_v1.clear();
            da_int_eq_v1.reserve( 10 );
            for ( unsigned i = 0; i < da_int_eq_v1.capacity(); ++i )
                da_int_eq_v1.push_back( i );

            TEST_ASSERT_EQUAL( da_int_eq_v0 == da_int_eq_v1, false );
            TEST_ASSERT_EQUAL( da_int_eq_v0 != da_int_eq_v1, true );

            // operator <, <=, >, >=
            cntl::DynamicArray<int> da_int_neq_v0, da_int_neq_v1;
            da_int_neq_v0.reserve( 5 );
            for ( unsigned i = 0; i < da_int_neq_v0.capacity(); ++i )
                da_int_neq_v0.push_back( i );

            da_int_neq_v1.reserve( 5 );
            for ( unsigned i = 0; i < da_int_neq_v1.capacity(); ++i )
                da_int_neq_v1.push_back( i );

            TEST_ASSERT_EQUAL( da_int_neq_v0 <  da_int_neq_v1, false );
            TEST_ASSERT_EQUAL( da_int_neq_v0 <= da_int_neq_v1, true );
            TEST_ASSERT_EQUAL( da_int_neq_v0 >  da_int_neq_v1, false );
            TEST_ASSERT_EQUAL( da_int_neq_v0 >= da_int_neq_v1, true );

            da_int_neq_v1.clear();
            da_int_neq_v1.reserve( 10 );
            for ( unsigned i = 0; i < da_int_neq_v1.capacity(); ++i )
                da_int_neq_v1.push_back( i );

            TEST_ASSERT_EQUAL( da_int_neq_v0 <  da_int_neq_v1, true );
            TEST_ASSERT_EQUAL( da_int_neq_v0 <= da_int_neq_v1, true );
            TEST_ASSERT_EQUAL( da_int_neq_v0 >  da_int_neq_v1, false );
            TEST_ASSERT_EQUAL( da_int_neq_v0 >= da_int_neq_v1, false );
        }
};

#endif
