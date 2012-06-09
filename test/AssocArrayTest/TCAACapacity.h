// -*- mode: C++; coding: utf-8 -*-
#ifndef TCAACAPACITY_H_INCLUDED
#define TCAACAPACITY_H_INCLUDED

#include <cstdlib>
#include <cntl/AssocArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCAACapacity : public Test::Case
{
    const float tolerance;

public:
    TCAACapacity( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            cntl::AssocArray<int,Vector3> aa_int_vec3_capacity;
            TEST_ASSERT( aa_int_vec3_capacity.size() == 0 );
            TEST_ASSERT( aa_int_vec3_capacity.empty() == true );

            TEST_ASSERT_EQUAL( aa_int_vec3_capacity.max_size(),
                               (cntl::AssocArray<int,Vector3>::size_type)(-1)/sizeof(cntl::AssocArray<int,Vector3>::value_type) );

            // [NOTE] reserve() : Non-standard API
            std::pair<int,Vector3> pair_array[] = { std::make_pair(3,Vector3(3,3,3)),
                                                    std::make_pair(2,Vector3(2,2,2)),
                                                    std::make_pair(1,Vector3(1,1,1)) };

            cntl::AssocArray<int,Vector3> aa_int_vec3_reserve;
            aa_int_vec3_reserve.reserve( 3 );
            TEST_ASSERT( aa_int_vec3_reserve.size() == 0 );
            TEST_ASSERT( aa_int_vec3_reserve.empty() == true );

            aa_int_vec3_reserve.insert( pair_array, pair_array+3 );
            TEST_ASSERT( aa_int_vec3_reserve.size() == 3 );
            TEST_ASSERT( aa_int_vec3_reserve.empty() == false );
        }
};

#endif
