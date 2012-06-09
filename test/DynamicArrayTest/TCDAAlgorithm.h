// -*- mode: C++; coding: utf-8 -*-
#ifndef TCDAALGORITHM_H_INCLUDED
#define TCDAALGORITHM_H_INCLUDED

#include <algorithm>
#include <numeric> // std::accumulate
#include <functional>
#include <cstdlib>

#include <cntl/DynamicArray.h>
#include <TestFramework.h>
#include "Vector3.h"

template <typename T>
struct StdTransformPredicate
{
    T operator ()( const T& x )
        {
            return 2 * x;
        }
};

template <typename T>
struct StdRemoveIfPredicate
{
    bool operator ()( const T& x )
        {
            return x % 2 == 0;
        }
};

template <typename T>
struct StdAccumulatePredicate
{
    T operator()( const T& v0, const T& v1 )
        {
            return v0 + v1;
        }
};

class TCDAAlgorithm : public Test::Case
{
    const float tolerance;

public:
    TCDAAlgorithm( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // transform
            cntl::DynamicArray<int> da_int_transform_source;
            cntl::DynamicArray<int> da_int_transform_result;

            da_int_transform_source.reserve( 10 );
            for ( unsigned i = 0; i < da_int_transform_source.capacity(); ++i )
                da_int_transform_source.push_back( i );

            // Ref.: Effective STL, Item 30
            // * std::back_inserter adds elements with push_back().
            // * std::front_inserter is usable only for the containers that support push_front().
            std::transform( da_int_transform_source.begin(), da_int_transform_source.end(),
                            std::back_inserter(da_int_transform_result), StdTransformPredicate<int>() );
            for ( unsigned i = 0; i < da_int_transform_result.size(); ++i )
                TEST_ASSERT_EQUAL( da_int_transform_result[i], 2 * i );


            // sort
            cntl::DynamicArray<int> da_int_stdsort;
            da_int_stdsort.reserve( 10 );
            for ( unsigned i = 0; i < da_int_stdsort.capacity(); ++i )
                da_int_stdsort.push_back( std::rand() );

            std::sort( da_int_stdsort.begin(), da_int_stdsort.end(), std::less<int>() );
            for ( unsigned i = 0; i < da_int_stdsort.size() - 1; ++i )
                TEST_ASSERT( da_int_stdsort[i] <= da_int_stdsort[i+1] );

            da_int_stdsort.clear();
            for ( unsigned i = 0; i < da_int_stdsort.capacity(); ++i )
                da_int_stdsort.push_back( std::rand() );
            std::sort( da_int_stdsort.begin(), da_int_stdsort.end(), std::greater<int>() );
            for ( unsigned i = 0; i < da_int_stdsort.size() - 1; ++i )
                TEST_ASSERT( da_int_stdsort[i] >= da_int_stdsort[i+1] );

            // remove
            cntl::DynamicArray<int> da_int_remove;
            da_int_remove.reserve( 10 );
            for ( unsigned i = 0; i < da_int_remove.capacity(); ++i )
                da_int_remove.push_back( i );

            cntl::DynamicArray<int>::iterator new_end = std::remove_if( da_int_remove.begin(), da_int_remove.end(), StdRemoveIfPredicate<int>() );
            TEST_ASSERT( da_int_remove.size() == 10 ); // std::remove and std::remove_if don't remove unnecessary elements, but move them out of the way.
            for ( cntl::DynamicArray<int>::iterator it = da_int_remove.begin(); it != new_end; ++it )
                TEST_ASSERT( (*it) % 2 != 0 );

            da_int_remove.erase( new_end, da_int_remove.end() );
            TEST_ASSERT( da_int_remove.size() == 5 );
            for ( cntl::DynamicArray<int>::iterator it = da_int_remove.begin(); it != da_int_remove.end(); ++it )
                TEST_ASSERT( (*it) % 2 != 0 );

            // lower_bound / upper_bound
            cntl::DynamicArray<int> da_int_binsearch;
            da_int_binsearch.reserve( 1000 );
            da_int_binsearch.push_back( 500 );
            da_int_binsearch.push_back( 500 );
            da_int_binsearch.push_back( 500 );
            for ( unsigned i = 0; i < da_int_binsearch.capacity()-3; ++i )
                da_int_binsearch.push_back( std::rand() % da_int_binsearch.capacity() );
            std::sort( da_int_binsearch.begin(), da_int_binsearch.end(), std::less<int>() );

            cntl::DynamicArray<int>::iterator itLowerboundResult, itUpperBoundResult;
            itLowerboundResult = std::lower_bound( da_int_binsearch.begin(), da_int_binsearch.end(), 500 );
            itUpperBoundResult = std::upper_bound( da_int_binsearch.begin(), da_int_binsearch.end(), 500 );
            TEST_ASSERT( itLowerboundResult < itUpperBoundResult );

            // set_difference
            cntl::DynamicArray<int> da_int_set0, da_int_set1;
            da_int_set0.reserve( 10 );
            da_int_set1.reserve( 10 );
            for ( unsigned i = 0; i < da_int_set0.capacity(); ++i )
                da_int_set0.push_back( i ); // 0, 1, 2, ..., 9

            for ( unsigned i = 0; i < da_int_set1.capacity(); ++i )
                da_int_set1.push_back( 2 * i ); // 0, 2, 4, ..., 18

            cntl::DynamicArray<int> da_int_set_diff_result;
            std::set_difference( da_int_set0.begin(), da_int_set0.end(),
                                 da_int_set1.begin(), da_int_set1.end(),
                                 std::back_inserter( da_int_set_diff_result ) );
            TEST_ASSERT( da_int_set_diff_result.size() == 5 ); // 1, 3, 5, 7, 9
            for ( unsigned i = 0; i < da_int_set_diff_result.size(); ++i )
                TEST_ASSERT( da_int_set_diff_result[i] % 2 != 0 );

            // accumulate
            cntl::DynamicArray<int> da_int_accumulate_source;
            da_int_accumulate_source.reserve( 10 );
            for ( unsigned i = 0; i < da_int_accumulate_source.capacity(); ++i )
                da_int_accumulate_source.push_back( i + 1 );

            int accumulate_result = std::accumulate( da_int_accumulate_source.begin(),
                                                     da_int_accumulate_source.end(),
                                                     0, StdAccumulatePredicate<int>() );
            TEST_ASSERT_EQUAL( accumulate_result, 55 );
      }
};

#endif
