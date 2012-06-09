// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSAITERATOR_H_INCLUDED
#define TCSAITERATOR_H_INCLUDED

#include <cstdlib>
#include <cntl/SortedArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCSAIterator : public Test::Case
{
    const float tolerance;

public:
    TCSAIterator( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            Vector3 vec3_array[] = { Vector3(3,3,3),
                                     Vector3(2,2,2),
                                     Vector3(1,1,1) };
            cntl::SortedArray<Vector3> sa_vec3_iterator( vec3_array, vec3_array+3 );

            int i;

            i = 1;
            for ( cntl::SortedArray<Vector3>::iterator it = sa_vec3_iterator.begin(); it != sa_vec3_iterator.end(); ++it, ++i )
            {
                TEST_ASSERT( (*it) == Vector3(float(i),float(i),float(i)) );
            }

            i = 3;
            for ( cntl::SortedArray<Vector3>::reverse_iterator rit = sa_vec3_iterator.rbegin(); rit != sa_vec3_iterator.rend(); ++rit, --i )
            {
                TEST_ASSERT( (*rit) == Vector3(float(i),float(i),float(i)) );
            }
        }
};

#endif
