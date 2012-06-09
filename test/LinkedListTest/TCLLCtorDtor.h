// -*- mode: C++; coding: utf-8 -*-
#ifndef TCLLCTORDTOR_H_INCLUDED
#define TCLLCTORDTOR_H_INCLUDED

#include <cstdlib>
#include <cntl/LinkedList.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCLLCtorDtor : public Test::Case
{
    const float tolerance;

public:
    TCLLCtorDtor( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            const int nNodeCapacity = 9;
            {
                cntl::LinkedList<int> ul_int_ctor_default;
                cntl::LinkedList<int>::iterator it = ul_int_ctor_default.begin();
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_int_ctor_default.insert( it, i+1 );
                }
                TEST_ASSERT( ul_int_ctor_default.size() == 2*nNodeCapacity );
            }

            {
                cntl::LinkedList<Vector3> ul_vec3_ctor_other;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3_ctor_other.end();
                    ul_vec3_ctor_other.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                cntl::LinkedList<Vector3> ul_vec3_ctor_copy( ul_vec3_ctor_other );
                TEST_ASSERT( ul_vec3_ctor_copy.size() == ul_vec3_ctor_other.size() );
            }
        }
};

#endif
