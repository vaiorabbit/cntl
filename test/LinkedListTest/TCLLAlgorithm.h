// -*- mode: C++; coding: utf-8 -*-
#ifndef TCLLALGORITHM_H_INCLUDED
#define TCLLALGORITHM_H_INCLUDED

#include <cstdlib>
#include <cntl/LinkedList.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCLLAlgorithm : public Test::Case
{
    const float tolerance;

    // [NOTE] GCC does not allow to pass locally-defined functors
    // as template parameters.
    // Ref.: http://www.codeguru.com/forum/showthread.php?t=471728

    struct UniquePredicate
    {
        bool operator ()( const Vector3& lhs, const Vector3& rhs )
            {
                return lhs == rhs;
            }
    };

    struct RemoveIfPredicate
    {
        bool operator ()( const Vector3& val )
            {
                return int(val.x) % 2 == 1;
            }
    };

    struct MergePredicate
    {
        bool operator ()( double lhs, double rhs )
            {
                return lhs > rhs;
            }
    };

    struct SortPredicate
    {
        bool operator ()( double lhs, double rhs )
            {
                return lhs > rhs;
            }
    };

public:
    TCLLAlgorithm( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            const int nNodeCapacity = 9;

            // reverse
            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 2*nNodeCapacity; ++i )
                {
                    it = ul_vec3.end();
                    ul_vec3.insert( it, Vector3(float(i+1),float(i+1),float(i+1)) );
                }
                ul_vec3.reverse();
                TEST_ASSERT( ul_vec3.front() == Vector3(2*nNodeCapacity,2*nNodeCapacity,2*nNodeCapacity) );
            }

            // unique
            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 10; ++i )
                {
                    for ( int n = 0; n < i+1; ++n )
                        ul_vec3.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }
                TEST_ASSERT( ul_vec3.size() == 55 );
                ul_vec3.unique();
                TEST_ASSERT( ul_vec3.size() == 10 );
            }

            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 9; i >= 0; --i )
                {
                    for ( int n = 0; n < i+1; ++n )
                        ul_vec3.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }
                TEST_ASSERT( ul_vec3.size() == 55 );
                ul_vec3.unique( UniquePredicate() );
                TEST_ASSERT( ul_vec3.size() == 10 );
            }

            // remove
            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 10; ++i )
                {
                    for ( int n = 0; n < i+1; ++n )
                        ul_vec3.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }
                TEST_ASSERT( ul_vec3.size() == 55 );

                ul_vec3.remove( Vector3(10,10,10) );
                TEST_ASSERT( ul_vec3.size() == 45 );

                ul_vec3.remove( Vector3(5,5,5) );
                TEST_ASSERT( ul_vec3.size() == 40 );
            }

            // remove_if
            {
                cntl::LinkedList<Vector3> ul_vec3;
                cntl::LinkedList<Vector3>::iterator it;
                for ( int i = 0; i < 10; ++i )
                {
                    for ( int n = 0; n < i+1; ++n )
                        ul_vec3.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }
                TEST_ASSERT( ul_vec3.size() == 55 );

                ul_vec3.remove_if( RemoveIfPredicate() );
                TEST_ASSERT( ul_vec3.size() == 30 );
            }

            // splice
            {
                cntl::LinkedList<Vector3> ul_vec3;
                for ( int i = 0; i < 5; ++i )
                {
                    ul_vec3.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                cntl::LinkedList<Vector3> ul_vec3_other;
                for ( int i = 5; i < 10; ++i )
                {
                    ul_vec3_other.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                ul_vec3.splice( ul_vec3.end(), ul_vec3_other );
                TEST_ASSERT( ul_vec3.size() == 10 );
                TEST_ASSERT( ul_vec3_other.empty() );
                int i = 1;
                for ( cntl::LinkedList<Vector3>::iterator it = ul_vec3.begin(); it != ul_vec3.end(); ++it )
                {
                    TEST_ASSERT( (*it).x == float(i) );
                    ++i;
                }
            }

            {
                cntl::LinkedList<Vector3> ul_vec3;
                for ( int i = 0; i < 5; ++i )
                {
                    ul_vec3.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                cntl::LinkedList<Vector3> ul_vec3_other;
                for ( int i = 5; i < 10; ++i )
                {
                    ul_vec3_other.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                ul_vec3.splice( ul_vec3.end(), ul_vec3_other, ul_vec3_other.begin() );
                TEST_ASSERT( ul_vec3.size() == 6 );
                TEST_ASSERT( ul_vec3_other.size() == 4 );
                TEST_ASSERT( ul_vec3.back() == Vector3(6,6,6) );
            }

            {
                cntl::LinkedList<Vector3> ul_vec3;
                for ( int i = 0; i < 5; ++i )
                {
                    ul_vec3.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                cntl::LinkedList<Vector3> ul_vec3_other;
                for ( int i = 5; i < 10; ++i )
                {
                    ul_vec3_other.push_back( Vector3(float(i+1),float(i+1),float(i+1)) );
                }

                ul_vec3.splice( ul_vec3.end(), ul_vec3_other, ul_vec3_other.begin(), ul_vec3_other.end() );
                TEST_ASSERT( ul_vec3.size() == 10 );
                TEST_ASSERT( ul_vec3_other.empty() );
            }

            // merge
            {
                cntl::LinkedList<double> ul_double_first, ul_double_second;
                ul_double_first.push_back( 2.2 );
                ul_double_first.push_back( 3.1 );
                ul_double_first.push_back( 3.7 );
                ul_double_second.push_back( 1.4 );
                ul_double_second.push_back( 2.9 );
                ul_double_second.push_back( 7.1 );

                ul_double_first.merge( ul_double_second );

                TEST_ASSERT( ul_double_first.size() == 6 );
                TEST_ASSERT( ul_double_second.size() == 0 );
            }

            {
                cntl::LinkedList<double> ul_double_first, ul_double_second;
                ul_double_first.push_back( 3.7 );
                ul_double_first.push_back( 3.1 );
                ul_double_first.push_back( 2.2 );
                ul_double_second.push_back( 7.1 );
                ul_double_second.push_back( 2.9 );
                ul_double_second.push_back( 1.4 );

                ul_double_first.merge( ul_double_second, MergePredicate() );

                TEST_ASSERT( ul_double_first.size() == 6 );
                TEST_ASSERT( ul_double_second.size() == 0 );
            }

            // sort
            {
                int n = 50;
                cntl::LinkedList<double> ul_double_sort;
                for ( int i = 0; i < n; ++i )
                    ul_double_sort.push_back( n * (double)std::rand()/RAND_MAX );

                ul_double_sort.sort();

                TEST_ASSERT( ul_double_sort.size() == n );
                for ( cntl::LinkedList<double>::iterator it = ul_double_sort.begin(); it != ul_double_sort.end(); ++it )
                {
                    cntl::LinkedList<double>::iterator it_prev = it++;
                    if ( it == ul_double_sort.end() )
                        break;
                    TEST_ASSERT( *it_prev <= *it );
                }
            }

            {
                int n = 50;
                cntl::LinkedList<double> ul_double_sort;
                for ( int i = 0; i < n; ++i )
                    ul_double_sort.push_back( n * (double)std::rand()/RAND_MAX );

                ul_double_sort.sort( SortPredicate() );

                TEST_ASSERT( ul_double_sort.size() == n );
                for ( cntl::LinkedList<double>::iterator it = ul_double_sort.begin(); it != ul_double_sort.end(); ++it )
                {
                    cntl::LinkedList<double>::iterator it_prev = it++;
                    if ( it == ul_double_sort.end() )
                        break;
                    TEST_ASSERT( *it_prev >= *it );
                }
            }
       }
};

#endif
