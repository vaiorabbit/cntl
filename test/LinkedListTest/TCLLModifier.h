// -*- mode: C++; coding: utf-8 -*-
#ifndef TCLLMODIFIER_H_INCLUDED
#define TCLLMODIFIER_H_INCLUDED

#include <cstdlib>
#include <cntl/DynamicArray.h>
#include <cntl/LinkedList.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCLLModifier : public Test::Case
{
    const float tolerance;

public:
    TCLLModifier( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            const int nNodeCapacity = 9;

            // insert(pos,val)
            {
                cntl::LinkedList<int> ul_int;
                cntl::LinkedList<int>::iterator it = ul_int.begin();
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_int.insert( it, i+1 );
                }
                TEST_ASSERT( ul_int.size() == 2*nNodeCapacity );
            }

            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }
                TEST_ASSERT( ul_vec3.size() == 2*nNodeCapacity );
            }

            // insert(pos,count,val)
            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                it = ul_vec3.end();
                ul_vec3.insert( it, 2*nNodeCapacity, Vector3(1,1,1) );
                TEST_ASSERT( ul_vec3.size() == 2*nNodeCapacity );
            }

            // insert(pos,first,last)
            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                it = ul_vec3.end();

                cntl::DynamicArray<Vector3> da_vec3;
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                    da_vec3.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );

                ul_vec3.insert( it, da_vec3.begin(), da_vec3.end() );

                TEST_ASSERT( ul_vec3.size() == 2*nNodeCapacity );
            }


            // erase(pos)
            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                it = ul_vec3.begin();
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3.erase( it );
                    TEST_ASSERT( ul_vec3.size() == 2*nNodeCapacity - (i+1) );
                }
                TEST_ASSERT( ul_vec3.empty() );
            }

            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                it = --(ul_vec3.end());
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3.erase( it );
                    TEST_ASSERT( it == ul_vec3.end() );
                    if ( !ul_vec3.empty() )
                        --it;
                    TEST_ASSERT( ul_vec3.size() == 2*nNodeCapacity - (i+1) );
                }
                TEST_ASSERT( ul_vec3.empty() );
            }

            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                it = ul_vec3.begin();
                for ( int i = 0; i < 9; ++i )
                    ++it;
                it = ul_vec3.erase( it );
                TEST_ASSERT( ul_vec3.size() == 2*nNodeCapacity - 1 );
            }

            // erase(first, last)
            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                cntl::LinkedList<Vector3>::iterator first = ul_vec3.begin();
                cntl::LinkedList<Vector3>::iterator last =ul_vec3.end();
                ul_vec3.erase( first, last );
                TEST_ASSERT( ul_vec3.size() == 0 );
            }

            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                cntl::LinkedList<Vector3>::iterator first = ul_vec3.begin();
                ++first;
                cntl::LinkedList<Vector3>::iterator last = --(ul_vec3.end());
                ul_vec3.erase( first, last );
                TEST_ASSERT( ul_vec3.size() == 2 );
                TEST_ASSERT( ul_vec3.front() == Vector3(1,1,1) );
                TEST_ASSERT( ul_vec3.back() == Vector3(2*nNodeCapacity,2*nNodeCapacity,2*nNodeCapacity) );
            }

            {
                const int cap = 9;
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*cap; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                cntl::LinkedList<Vector3>::iterator first = ul_vec3.begin();
                for ( int i = 0; i < 4; ++i )
                    ++first;
                cntl::LinkedList<Vector3>::iterator last = ul_vec3.end();
                for ( int i = 0; i < 13; ++i )
                    --last;
                it = ul_vec3.erase( first, last );
                TEST_ASSERT( ul_vec3.size() == 2*cap - 1 );
                TEST_ASSERT( *it == Vector3(6,6,6) );
            }

            {
                const int cap = 9;
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*cap; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                cntl::LinkedList<Vector3>::iterator first = ul_vec3.begin();
                for ( int i = 0; i < 6; ++i )
                    ++first;
                cntl::LinkedList<Vector3>::iterator last = ul_vec3.end();
                for ( int i = 0; i < 11; ++i )
                    --last;
                it = ul_vec3.erase( first, last );
                TEST_ASSERT( ul_vec3.size() == 2*cap - 1 );
                TEST_ASSERT( *it == Vector3(8,8,8) );
            }

            // push_front, pop_front, push_back, pop_back
            {
                const int cap = 8;
                cntl::LinkedList<Vector3> ul_vec3;

                for ( int i = 0; i < 10; ++i )
                {
                    ul_vec3.push_front( Vector3(float(i+1),float(i+1),float(i+1)) );
                }
                TEST_ASSERT( ul_vec3.size() == 10 );
                TEST_ASSERT( ul_vec3.front() == Vector3(10,10,10) );

                for ( int i = 0; i < 5; ++i )
                {
                    ul_vec3.pop_front();
                }
                TEST_ASSERT( ul_vec3.size() == 5 );
                TEST_ASSERT( ul_vec3.front() == Vector3(5,5,5) );

                for ( int i = 0; i < 5; ++i )
                {
                    ul_vec3.pop_front();
                }
                TEST_ASSERT( ul_vec3.empty() );

                for ( int i = 0; i < 10; ++i )
                {
                    ul_vec3.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }
                TEST_ASSERT( ul_vec3.size() == 10 );
                TEST_ASSERT( ul_vec3.back() == Vector3(10,10,10) );

                for ( int i = 0; i < 5; ++i )
                {
                    ul_vec3.pop_back();
                }
                TEST_ASSERT( ul_vec3.size() == 5 );
                TEST_ASSERT( ul_vec3.back() == Vector3(5,5,5) );

                for ( int i = 0; i < 5; ++i )
                {
                    ul_vec3.pop_back();
                }
                TEST_ASSERT( ul_vec3.empty() );
            }

            // assign
            {
                const int cap = 8;
                cntl::LinkedList<Vector3> ul_vec3_assign;

                for ( int i = 0; i < 10; ++i )
                {
                    ul_vec3_assign.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                cntl::LinkedList<Vector3> ul_vec3_0;
                for ( int i = 0; i < 20; ++i )
                {
                    ul_vec3_0.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                ul_vec3_assign.assign( ul_vec3_0.begin(), ul_vec3_0.end() );
                TEST_ASSERT( ul_vec3_assign.size() == 20 );
                TEST_ASSERT( ul_vec3_assign.back() == Vector3(20,20,20) );
            }

            {
                const int cap = 8;
                cntl::LinkedList<Vector3> ul_vec3_assign;

                for ( int i = 0; i < 20; ++i )
                {
                    ul_vec3_assign.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                cntl::LinkedList<Vector3> ul_vec3_0;
                for ( int i = 0; i < 10; ++i )
                {
                    ul_vec3_0.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                ul_vec3_assign.assign( ul_vec3_0.begin(), ul_vec3_0.end() );
                TEST_ASSERT( ul_vec3_assign.size() == 10 );
                TEST_ASSERT( ul_vec3_assign.back() == Vector3(10,10,10) );
            }

            {
                const int cap = 8;
                cntl::LinkedList<Vector3> ul_vec3_assign;

                for ( int i = 0; i < 10; ++i )
                {
                    ul_vec3_assign.push_back( Vector3(-1,-1,-1) );
                }

                cntl::LinkedList<Vector3> ul_vec3_0;
                for ( int i = 0; i < 10; ++i )
                {
                    ul_vec3_0.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                ul_vec3_assign.assign( ul_vec3_0.begin(), ul_vec3_0.end() );
                TEST_ASSERT( ul_vec3_assign.size() == 10 );
                TEST_ASSERT( ul_vec3_assign.back() == Vector3(10,10,10) );
            }
        }
};

#endif
