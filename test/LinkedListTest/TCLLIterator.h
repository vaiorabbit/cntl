// -*- mode: C++; coding: utf-8 -*-
#ifndef TCLLITERATOR_H_INCLUDED
#define TCLLITERATOR_H_INCLUDED

#include <cstdlib>
#include <cntl/LinkedList.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCLLIterator : public Test::Case
{
    const float tolerance;

public:
    TCLLIterator( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            const int nNodeCapacity = 9;

            cntl::LinkedList<Vector3> ul_vec3;
            cntl::LinkedList<Vector3>::iterator it;
            for ( int i = 0; i < 2*nNodeCapacity; ++i )
            {
                it = ul_vec3.end();
                ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
            }

            int i;

            i = 1;
            for ( cntl::LinkedList<Vector3>::iterator it = ul_vec3.begin(); it != ul_vec3.end(); ++it, ++i )
            {
                TEST_ASSERT( *it == Vector3(float(i),float(i),float(i)) );
            }

            i = 2*nNodeCapacity;
            for ( cntl::LinkedList<Vector3>::reverse_iterator it = ul_vec3.rbegin(); it != ul_vec3.rend(); ++it, --i )
            {
                TEST_ASSERT( *it == Vector3(float(i),float(i),float(i)) );
            }
        }
};

#endif
