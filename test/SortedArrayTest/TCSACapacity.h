// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSACAPACITY_H_INCLUDED
#define TCSACAPACITY_H_INCLUDED

#include <cstdlib>
#include <cntl/SortedArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCSACapacity : public Test::Case
{
    const float tolerance;

public:
    TCSACapacity( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            cntl::SortedArray<Vector3> sa_vec3_capacity;
            TEST_ASSERT( sa_vec3_capacity.size() == 0 );
            TEST_ASSERT( sa_vec3_capacity.empty() == true );

            TEST_ASSERT_EQUAL( sa_vec3_capacity.max_size(),
                               (cntl::SortedArray<Vector3>::size_type)(-1)/sizeof(cntl::SortedArray<Vector3>::value_type) );

            // [NOTE] reserve() : Non-standard API
            Vector3 pair_array[] = { Vector3(3,3,3),
                                     Vector3(2,2,2),
                                     Vector3(1,1,1) };

            cntl::SortedArray<Vector3> sa_vec3_reserve;
            sa_vec3_reserve.reserve( 3 );
            TEST_ASSERT( sa_vec3_reserve.size() == 0 );
            TEST_ASSERT( sa_vec3_reserve.empty() == true );

            sa_vec3_reserve.insert( pair_array, pair_array+3 );
            TEST_ASSERT( sa_vec3_reserve.size() == 3 );
            TEST_ASSERT( sa_vec3_reserve.empty() == false );
        }
};

#endif
