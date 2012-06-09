// -*- mode: C++; coding: utf-8 -*-
#ifndef TCDAITERATOR_H_INCLUDED
#define TCDAITERATOR_H_INCLUDED

#include <cntl/DynamicArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCDAIterator : public Test::Case
{
    const float tolerance;

public:
    TCDAIterator( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            cntl::DynamicArray<Vector3> da_vec3_iter_init;
            TEST_ASSERT_EQUAL( da_vec3_iter_init.begin(), (cntl::DynamicArray<Vector3>::iterator)0 );
            TEST_ASSERT_EQUAL( da_vec3_iter_init.end(), (cntl::DynamicArray<Vector3>::iterator)0 );
            TEST_ASSERT_EQUAL( da_vec3_iter_init.rbegin(), (cntl::DynamicArray<Vector3>::reverse_iterator)0 );
            TEST_ASSERT_EQUAL( da_vec3_iter_init.rend(), (cntl::DynamicArray<Vector3>::reverse_iterator)0 );

            Vector3 vec_array[] = { Vector3(0,0,0), Vector3(1,1,1), Vector3(2,2,2) };
            cntl::DynamicArray<Vector3> da_vec3_loop( vec_array, vec_array+3 );

            int count;

            count = 0;
            for ( cntl::DynamicArray<Vector3>::iterator it = da_vec3_loop.begin(); it != da_vec3_loop.end(); ++it, ++count )
            {
                TEST_ASSERT_DOUBLES_EQUAL( (*it).x, (float)count, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( (*it).y, (float)count, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( (*it).z, (float)count, tolerance );
            }

            count = 2;
            for ( cntl::DynamicArray<Vector3>::reverse_iterator rit = da_vec3_loop.rbegin(); rit != da_vec3_loop.rend(); ++rit, --count )
            {
                TEST_ASSERT_DOUBLES_EQUAL( (*rit).x, (float)count, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( (*rit).y, (float)count, tolerance );
                TEST_ASSERT_DOUBLES_EQUAL( (*rit).z, (float)count, tolerance );
            }

            // compare
            cntl::DynamicArray<Vector3>::iterator it = da_vec3_loop.begin();
            cntl::DynamicArray<Vector3>::const_iterator cit = da_vec3_loop.begin();
            TEST_ASSERT( it == cit );
            TEST_ASSERT( cit == it );
        }
};

#endif
