// -*- mode: C++; coding: utf-8 -*-
#ifndef TCAAITERATOR_H_INCLUDED
#define TCAAITERATOR_H_INCLUDED

#include <cstdlib>
#include <cntl/AssocArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCAAIterator : public Test::Case
{
    const float tolerance;

public:
    TCAAIterator( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            std::pair<int,Vector3> pair_array[] = { std::make_pair(3,Vector3(3,3,3)),
                                                    std::make_pair(2,Vector3(2,2,2)),
                                                    std::make_pair(1,Vector3(1,1,1)) };
            cntl::AssocArray<int,Vector3> aa_int_vec3_iterator( pair_array, pair_array+3 );

            int i;

            i = 1;
            for ( cntl::AssocArray<int,Vector3>::iterator it = aa_int_vec3_iterator.begin(); it != aa_int_vec3_iterator.end(); ++it, ++i )
            {
                TEST_ASSERT( (*it).first == i );
                TEST_ASSERT( (*it).second == Vector3(float(i),float(i),float(i)) );
            }

            i = 3;
            for ( cntl::AssocArray<int,Vector3>::reverse_iterator rit = aa_int_vec3_iterator.rbegin(); rit != aa_int_vec3_iterator.rend(); ++rit, --i )
            {
                TEST_ASSERT( (*rit).first == i );
                TEST_ASSERT( (*rit).second == Vector3(float(i),float(i),float(i)) );
            }
        }
};

#endif
