// -*- mode: C++; coding: utf-8 -*-
#ifndef TCLLOPERATOR_H_INCLUDED
#define TCLLOPERATOR_H_INCLUDED

#include <cstdlib>
#include <cntl/LinkedList.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCLLOperator : public Test::Case
{
    const float tolerance;

public:
    TCLLOperator( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            const int nNodeCapacity = 9;

            // ==, !=, swap
            {
                cntl::LinkedList<Vector3> ul_vec3_lhs, ul_vec3_rhs;
                for ( int i = 0; i < 10; ++i )
                {
                    for ( int n = 0; n < i+1; ++n )
                        ul_vec3_lhs.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                    for ( int n = 0; n < i+1; ++n )
                        ul_vec3_rhs.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                TEST_ASSERT( ul_vec3_lhs == ul_vec3_rhs );
                TEST_ASSERT( !(ul_vec3_lhs != ul_vec3_rhs) );
                swap( ul_vec3_lhs, ul_vec3_rhs );
                TEST_ASSERT( ul_vec3_lhs == ul_vec3_rhs );
                TEST_ASSERT( !(ul_vec3_lhs != ul_vec3_rhs) );

                ul_vec3_rhs.pop_back();
                TEST_ASSERT( ul_vec3_lhs != ul_vec3_rhs );
                TEST_ASSERT( !(ul_vec3_lhs == ul_vec3_rhs) );
            }

            // <, <=, >, >=
            {
                cntl::LinkedList<Vector3> ul_vec3_lhs, ul_vec3_rhs;
                for ( int i = 0; i < 10; ++i )
                {
                    for ( int n = 0; n < i+1; ++n )
                        ul_vec3_lhs.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                    for ( int n = 0; n < i+1; ++n )
                        ul_vec3_rhs.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                TEST_ASSERT( ul_vec3_lhs <= ul_vec3_rhs );
                TEST_ASSERT( ul_vec3_lhs >= ul_vec3_rhs );

                ul_vec3_rhs.pop_back();
                TEST_ASSERT( ul_vec3_lhs > ul_vec3_rhs );
                TEST_ASSERT( ul_vec3_rhs < ul_vec3_lhs );
                TEST_ASSERT( ul_vec3_lhs >= ul_vec3_rhs );
                TEST_ASSERT( ul_vec3_rhs <= ul_vec3_lhs );
            }
       }
};

#endif
