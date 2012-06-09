// -*- mode: C++; coding: utf-8 -*-
#ifndef TCDARESIZERESERVE_H_INCLUDED
#define TCDARESIZERESERVE_H_INCLUDED

#include <cntl/DynamicArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCDAResizeReserve : public Test::Case
{
    const float tolerance;

public:
    TCDAResizeReserve( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            cntl::DynamicArray<Vector3> da_vec3_default;
            TEST_ASSERT( da_vec3_default.size() == 0 );

            da_vec3_default.reserve( 10 );
            TEST_ASSERT( da_vec3_default.size() == 0 );
            TEST_ASSERT( da_vec3_default.capacity() == 10 );

            da_vec3_default.resize( 5 );
            TEST_ASSERT( da_vec3_default.size() == 5 );
            TEST_ASSERT( da_vec3_default.capacity() == 10 );

            // shrink-to-fit idiom
            cntl::DynamicArray<Vector3>( da_vec3_default ).swap( da_vec3_default );
            TEST_ASSERT( da_vec3_default.size() == 5 );
            TEST_ASSERT( da_vec3_default.capacity() == 5 );

            // max_size : returns the maximum size of the DynamicArray.
            TEST_ASSERT( da_vec3_default.max_size() == cntl::DynamicArray<Vector3>::size_type(-1)/sizeof(Vector3) );
        }
};

#endif
