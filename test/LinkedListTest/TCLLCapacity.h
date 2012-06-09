// -*- mode: C++; coding: utf-8 -*-
#ifndef TCLLCAPACITY_H_INCLUDED
#define TCLLCAPACITY_H_INCLUDED

#include <cstdlib>
#include <cntl/LinkedList.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCLLCapacity : public Test::Case
{
    const float tolerance;

public:
    TCLLCapacity( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            const int nNodeCapacity = 9;

            // empty
            {
                cntl::LinkedList<int> ul_int;
                TEST_ASSERT( ul_int.empty() );
                cntl::LinkedList<int>::iterator it = ul_int.begin();
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_int.insert( it, i+1 );
                }
                TEST_ASSERT( !ul_int.empty() );
            }

            // size
            {
                cntl::LinkedList<int> ul_int;
                TEST_ASSERT( ul_int.size() == 0 );
                cntl::LinkedList<int>::iterator it = ul_int.begin();
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_int.insert( it, i+1 );
                }
                TEST_ASSERT( ul_int.size() == 2*nNodeCapacity );
            }

            // max_size
            {
                cntl::LinkedList<int> ul_int;
                TEST_ASSERT( ul_int.size() == 0 );
                cntl::LinkedList<int>::iterator it = ul_int.begin();
                for ( int i = 0; i < 20; ++i )
                {
                    it = ul_int.insert( it, i+1 );
                }
                TEST_ASSERT( ul_int.size() == 20 );

                TEST_ASSERT_EQUAL( ul_int.max_size(),
                                   (cntl::LinkedList<int>::size_type)(-1)/sizeof(cntl::LinkedList<int>::value_type) );
            }

            // resize
            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                ul_vec3.resize( 10 );
                TEST_ASSERT( ul_vec3.size() == 10 );

                ul_vec3.clear();
                for ( int i = 0; i < 10; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                ul_vec3.resize( 20, Vector3(100,100,100) );
                TEST_ASSERT( ul_vec3.size() == 20 );
                TEST_ASSERT( ul_vec3.back() == Vector3(100,100,100) );
            }
        }
};

#endif
