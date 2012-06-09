// -*- mode: C++; coding: utf-8 -*-
#ifndef TCDAACCESSOR_H_INCLUDED
#define TCDAACCESSOR_H_INCLUDED

#include <cntl/DynamicArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCDAAccessor : public Test::Case
{
    const float tolerance;

public:
    TCDAAccessor( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // front / back
            cntl::DynamicArray<Vector3> da_vec3_accessor;
            da_vec3_accessor.reserve( 3 );
            for ( int i = 0; i < 3; ++i )
                da_vec3_accessor.push_back( Vector3(float(i),float(i),float(i)) );

            Vector3 v0 = da_vec3_accessor.front();
            TEST_ASSERT_DOUBLES_EQUAL( v0.x, 0.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( v0.y, 0.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( v0.z, 0.0f, tolerance );

            Vector3 v2 = da_vec3_accessor.back();
            TEST_ASSERT_DOUBLES_EQUAL( v2.x, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( v2.y, 2.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( v2.z, 2.0f, tolerance );

            // at / operator[]
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_accessor.at(1).x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_accessor.at(1).y, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_accessor.at(1).z, 1.0f, tolerance );

            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_accessor[1].x, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_accessor[1].y, 1.0f, tolerance );
            TEST_ASSERT_DOUBLES_EQUAL( da_vec3_accessor[1].z, 1.0f, tolerance );
        }
};

#endif
