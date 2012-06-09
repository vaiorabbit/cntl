// -*- mode: C++; coding: utf-8 -*-
#ifndef TCDADELETION_H_INCLUDED
#define TCDADELETION_H_INCLUDED

#include <cntl/DynamicArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCDADeletion : public Test::Case
{
    const float tolerance;

public:
    TCDADeletion( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // empty(), clear()
            cntl::DynamicArray<Vector3> da_vec3_emptytest;
            TEST_ASSERT( da_vec3_emptytest.empty() );
            da_vec3_emptytest.erase(da_vec3_emptytest.begin(), da_vec3_emptytest.end());

            da_vec3_emptytest.reserve( 10 );
            TEST_ASSERT( da_vec3_emptytest.empty() );

            da_vec3_emptytest.resize( 10 );
            TEST_ASSERT( da_vec3_emptytest.empty() == false );

            da_vec3_emptytest.clear();
            TEST_ASSERT( da_vec3_emptytest.empty() );

            // erase( pos )
            Vector3 vec_array[] = {
                Vector3(0,0,0), Vector3(1,1,1), Vector3(2,2,2), Vector3(3,3,3), Vector3(4,4,4)
            };
            std::size_t vec_array_count = sizeof(vec_array)/sizeof(vec_array[0]);

            cntl::DynamicArray<Vector3> da_vec3_erase_at( vec_array, vec_array+vec_array_count );
            TEST_ASSERT( da_vec3_erase_at.size() == 5 );
            da_vec3_erase_at.erase( da_vec3_erase_at.begin() );
            da_vec3_erase_at.erase( da_vec3_erase_at.end()-1 );
            TEST_ASSERT( da_vec3_erase_at.size() == 3 );

            // erase( first, last ) : Removes elements in range [first,last).
            cntl::DynamicArray<Vector3> da_vec3_erase_range( vec_array, vec_array+vec_array_count );
            TEST_ASSERT( da_vec3_erase_range.size() == 5 );
            da_vec3_erase_range.erase( da_vec3_erase_range.begin()+1, da_vec3_erase_range.end()-1 );
            TEST_ASSERT( da_vec3_erase_range.size() == 2 );
        }
};

#endif
