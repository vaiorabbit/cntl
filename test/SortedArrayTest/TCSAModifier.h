// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSAMODIFIER_H_INCLUDED
#define TCSAMODIFIER_H_INCLUDED

#include <string>
#include <cstdlib>
#include <cntl/SortedArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCSAModifier : public Test::Case
{
    const float tolerance;

public:
    TCSAModifier( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // insert( x )
            {
                cntl::SortedArray<Vector3> sa_vec3_insert;
                for ( int i = 0; i < 100; ++i )
                {
                    unsigned r = std::rand() % 10;
                    sa_vec3_insert.insert( Vector3(float(r),float(r),float(r)) );
                }
                TEST_ASSERT( sa_vec3_insert.size() == 10 );

                int i = 0;
                for ( cntl::SortedArray<Vector3>::iterator it = sa_vec3_insert.begin(); it != sa_vec3_insert.end(); ++it, ++i )
                {
                    TEST_ASSERT( (*it) == Vector3(float(i),float(i),float(i)) );
                }
            }

            // insert( position, x )
            {
                cntl::SortedArray<Vector3> sa_vec3_insert;
                for ( int i = 0; i < 100; ++i )
                {
                    unsigned r = std::rand() % 10;
                    sa_vec3_insert.insert( sa_vec3_insert.end(), Vector3(float(r),float(r),float(r)) );
                }
                TEST_ASSERT( sa_vec3_insert.size() == 10 );

                int i = 0;
                for ( cntl::SortedArray<Vector3>::iterator it = sa_vec3_insert.begin(); it != sa_vec3_insert.end(); ++it, ++i )
                {
                    TEST_ASSERT( (*it) == Vector3(float(i),float(i),float(i)) );
                }
            }

            // insert( first, last )
            {
                Vector3 pair_array[] = { Vector3(3,3,3),
                                         Vector3(2,2,2),
                                         Vector3(1,1,1) };

                cntl::SortedArray<Vector3> aa_int_vec3_insert_range;
                TEST_ASSERT( aa_int_vec3_insert_range.size() == 0 );
                TEST_ASSERT( aa_int_vec3_insert_range.empty() == true );
                aa_int_vec3_insert_range.insert( pair_array, pair_array+3 );
                int i = 1;
                for ( cntl::SortedArray<Vector3>::iterator it = aa_int_vec3_insert_range.begin(); it != aa_int_vec3_insert_range.end(); ++it, ++i )
                {
                    TEST_ASSERT( (*it) == Vector3(float(i),float(i),float(i)) );
                }
            }

            // erase( iterator position )
            {
                std::wstring pair_array[] = { std::wstring(L"Ｘ"),
                                              std::wstring(L"Ｙ"),
                                              std::wstring(L"Ｚ") };
                cntl::SortedArray<std::wstring> sa_wstr_erase_pos;
                sa_wstr_erase_pos.insert( pair_array, pair_array+3 );

                sa_wstr_erase_pos.erase( sa_wstr_erase_pos.begin() + 1 );
                TEST_ASSERT( sa_wstr_erase_pos.size() == 2 );
                for ( cntl::SortedArray<std::wstring>::iterator it = sa_wstr_erase_pos.begin(); it != sa_wstr_erase_pos.end(); ++it )
                {
                    TEST_ASSERT( (*it) != std::wstring(L"Ｙ") );
                }
            }

            // erase( const key_type& x )
            {
                std::wstring pair_array[] = { std::wstring(L"Ｘ"),
                                              std::wstring(L"Ｙ"),
                                              std::wstring(L"Ｚ") };
                cntl::SortedArray<std::wstring> sa_wstr_erase_key;
                sa_wstr_erase_key.insert( pair_array, pair_array+3 );

                sa_wstr_erase_key.erase( L"Y" );
                TEST_ASSERT( sa_wstr_erase_key.size() == 3 );
                sa_wstr_erase_key.erase( L"Ｙ" );
                TEST_ASSERT( sa_wstr_erase_key.size() == 2 );
                for ( cntl::SortedArray<std::wstring>::iterator it = sa_wstr_erase_key.begin(); it != sa_wstr_erase_key.end(); ++it )
                {
                    TEST_ASSERT( (*it) != std::wstring(L"Ｙ") );
                }
            }

            // erase( first, last )
            {
                std::wstring pair_array[] = { std::wstring(L"Ｘ"),
                                              std::wstring(L"Ｙ"),
                                              std::wstring(L"Ｚ") };
                cntl::SortedArray<std::wstring> sa_wstr_erase_range;
                sa_wstr_erase_range.insert( pair_array, pair_array+3 );

                sa_wstr_erase_range.erase( sa_wstr_erase_range.begin(), sa_wstr_erase_range.end() );
                TEST_ASSERT( sa_wstr_erase_range.size() == 0 );
            }

            // swap (member function)
            {
                std::wstring wstrkey_pair_array0[] = { std::wstring(L"Ｘ"),
                                                       std::wstring(L"Ｙ"),
                                                       std::wstring(L"Ｚ") };

                cntl::SortedArray<std::wstring> sa_wstr_swap0;
                sa_wstr_swap0.insert( wstrkey_pair_array0, wstrkey_pair_array0+3 );

                std::wstring wstrkey_pair_array1[] = { std::wstring(L"A"),
                                                       std::wstring(L"B"),
                                                       std::wstring(L"C"),
                                                       std::wstring(L"D") };

                cntl::SortedArray<std::wstring> sa_wstr_swap1;
                sa_wstr_swap1.insert( wstrkey_pair_array1, wstrkey_pair_array1+4 );

                sa_wstr_swap0.swap( sa_wstr_swap1 );
                TEST_ASSERT( sa_wstr_swap0.size() == 4 );
                TEST_ASSERT( sa_wstr_swap1.size() == 3 );
                TEST_ASSERT( (*(sa_wstr_swap0.begin())) == std::wstring(L"A") );
                TEST_ASSERT( (*(sa_wstr_swap1.begin())) == std::wstring(L"Ｘ") );
            }

            // swap (cntl::swap)
            {
                std::wstring wstrkey_pair_array0[] = { std::wstring(L"Ｘ"),
                                                       std::wstring(L"Ｙ"),
                                                       std::wstring(L"Ｚ") };

                cntl::SortedArray<std::wstring> sa_wstr_swap0;
                sa_wstr_swap0.insert( wstrkey_pair_array0, wstrkey_pair_array0+3 );

                std::wstring wstrkey_pair_array1[] = { std::wstring(L"A"),
                                                       std::wstring(L"B"),
                                                       std::wstring(L"C"),
                                                       std::wstring(L"D") };

                cntl::SortedArray<std::wstring> sa_wstr_swap1;
                sa_wstr_swap1.insert( wstrkey_pair_array1, wstrkey_pair_array1+4 );

                cntl::swap( sa_wstr_swap0, sa_wstr_swap1 );
                TEST_ASSERT( sa_wstr_swap0.size() == 4 );
                TEST_ASSERT( sa_wstr_swap1.size() == 3 );
                TEST_ASSERT( (*(sa_wstr_swap0.begin())) == std::wstring(L"A") );
                TEST_ASSERT( (*(sa_wstr_swap1.begin())) == std::wstring(L"Ｘ") );
            }

            // clear
            {
                std::wstring pair_array[] = { std::wstring(L"Ｘ"),
                                              std::wstring(L"Ｙ"),
                                              std::wstring(L"Ｚ") };

                cntl::SortedArray<std::wstring> sa_wstr_clear;
                sa_wstr_clear.insert( pair_array, pair_array+3 );
                sa_wstr_clear.clear();
                TEST_ASSERT( sa_wstr_clear.size() == 0 );
            }
        }
};

#endif
