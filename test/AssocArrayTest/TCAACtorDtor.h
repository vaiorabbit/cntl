// -*- mode: C++; coding: utf-8 -*-
#ifndef TCAACTORDTOR_H_INCLUDED
#define TCAACTORDTOR_H_INCLUDED

#include <cstdlib>
#include <cntl/AssocArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCAACtorDtor : public Test::Case
{
    const float tolerance;

public:
    TCAACtorDtor( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            cntl::AssocArray<int, Vector3> aa_int_vec3_default;
            TEST_ASSERT( aa_int_vec3_default.size() == 0 );

            std::pair<int,Vector3> pair_array[] = { std::make_pair(3,Vector3(3,3,3)),
                                                    std::make_pair(2,Vector3(2,2,2)),
                                                    std::make_pair(1,Vector3(1,1,1)) };
            cntl::AssocArray<int,Vector3> aa_int_vec3_range( pair_array, pair_array+3 );
            TEST_ASSERT( aa_int_vec3_range.size() == 3 );

            cntl::AssocArray<int,Vector3> aa_int_vec3_copyctor( aa_int_vec3_range );
            TEST_ASSERT( aa_int_vec3_range.size() == 3 );
            TEST_ASSERT( aa_int_vec3_copyctor == aa_int_vec3_range );

            cntl::AssocArray<int,Vector3> aa_int_vec3_copied;
            aa_int_vec3_copied = aa_int_vec3_range;
            TEST_ASSERT( aa_int_vec3_range.size() == 3 );
            TEST_ASSERT( aa_int_vec3_copied == aa_int_vec3_range );
        }
};

#endif
