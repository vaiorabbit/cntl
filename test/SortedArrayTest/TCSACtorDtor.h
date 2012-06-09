// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSACTORDTOR_H_INCLUDED
#define TCSACTORDTOR_H_INCLUDED

#include <cstdlib>
#include <cntl/SortedArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCSACtorDtor : public Test::Case
{
    const float tolerance;

public:
    TCSACtorDtor( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            cntl::SortedArray<Vector3> sa_vec3_default;
            TEST_ASSERT( sa_vec3_default.size() == 0 );

            Vector3 pair_array[] = { Vector3(3,3,3),
                                     Vector3(2,2,2),
                                     Vector3(1,1,1) };
            cntl::SortedArray<Vector3> sa_vec3_range( pair_array, pair_array+3 );
            TEST_ASSERT( sa_vec3_range.size() == 3 );

            cntl::SortedArray<Vector3> sa_vec3_copyctor( sa_vec3_range );
            TEST_ASSERT( sa_vec3_range.size() == 3 );
            TEST_ASSERT( sa_vec3_copyctor == sa_vec3_range );

            cntl::SortedArray<Vector3> sa_vec3_copied;
            sa_vec3_copied = sa_vec3_range;
            TEST_ASSERT( sa_vec3_range.size() == 3 );
            TEST_ASSERT( sa_vec3_copied == sa_vec3_range );
        }
};

#endif
