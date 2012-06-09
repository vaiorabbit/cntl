// -*- mode: C++; coding: utf-8 -*-
#ifndef TCAAMODIFIER_H_INCLUDED
#define TCAAMODIFIER_H_INCLUDED

#include <string>
#include <cstdlib>
#include <cntl/AssocArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCAAModifier : public Test::Case
{
    const float tolerance;

public:
    TCAAModifier( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // insert( x )
            {
                cntl::AssocArray<int,Vector3> aa_int_vec3_insert;
                for ( int i = 0; i < 100; ++i )
                {
                    unsigned r = std::rand() % 10;
                    aa_int_vec3_insert.insert( std::make_pair(r,Vector3(float(r),float(r),float(r))) );
                }
                TEST_ASSERT( aa_int_vec3_insert.size() == 10 );

                int i = 0;
                for ( cntl::AssocArray<int,Vector3>::iterator it = aa_int_vec3_insert.begin(); it != aa_int_vec3_insert.end(); ++it, ++i )
                {
                    TEST_ASSERT( (*it).first == i );
                    TEST_ASSERT( (*it).second == Vector3(float(i),float(i),float(i)) );
                }
            }

            // insert( position, x )
            {
                cntl::AssocArray<int,Vector3> aa_int_vec3_insert;
                for ( int i = 0; i < 100; ++i )
                {
                    unsigned r = std::rand() % 10;
                    aa_int_vec3_insert.insert( aa_int_vec3_insert.end(), std::make_pair(r,Vector3(float(r),float(r),float(r))) );
                }
                TEST_ASSERT( aa_int_vec3_insert.size() == 10 );

                int i = 0;
                for ( cntl::AssocArray<int,Vector3>::iterator it = aa_int_vec3_insert.begin(); it != aa_int_vec3_insert.end(); ++it, ++i )
                {
                    TEST_ASSERT( (*it).first == i );
                    TEST_ASSERT( (*it).second == Vector3(float(i),float(i),float(i)) );
                }
            }

            // insert( first, last )
            {
                std::pair<int,Vector3> pair_array[] = { std::make_pair(3,Vector3(3,3,3)),
                                                        std::make_pair(2,Vector3(2,2,2)),
                                                        std::make_pair(1,Vector3(1,1,1)) };

                cntl::AssocArray<int,Vector3> aa_int_vec3_insert_range;
                TEST_ASSERT( aa_int_vec3_insert_range.size() == 0 );
                TEST_ASSERT( aa_int_vec3_insert_range.empty() == true );
                int i = 1;
                for ( cntl::AssocArray<int,Vector3>::iterator it = aa_int_vec3_insert_range.begin(); it != aa_int_vec3_insert_range.end(); ++it, ++i )
                {
                    TEST_ASSERT( (*it).first == i );
                    TEST_ASSERT( (*it).second == Vector3(float(i),float(i),float(i)) );
                }
            }

            // erase( iterator position )
            {
                std::pair<std::wstring,Vector3> pair_array[] = { std::make_pair(std::wstring(L"Ｘ"),Vector3(1,0,0)),
                                                                 std::make_pair(std::wstring(L"Ｙ"),Vector3(0,1,0)),
                                                                 std::make_pair(std::wstring(L"Ｚ"),Vector3(0,0,1)) };
                cntl::AssocArray<std::wstring,Vector3> aa_wstr_vec3_erase_pos;
                aa_wstr_vec3_erase_pos.insert( pair_array, pair_array+3 );

                aa_wstr_vec3_erase_pos.erase( aa_wstr_vec3_erase_pos.begin() + 1 );
                TEST_ASSERT( aa_wstr_vec3_erase_pos.size() == 2 );
                for ( cntl::AssocArray<std::wstring,Vector3>::iterator it = aa_wstr_vec3_erase_pos.begin(); it != aa_wstr_vec3_erase_pos.end(); ++it )
                {
                    TEST_ASSERT( (*it).first != std::wstring(L"Ｙ") );
                }
            }

            // erase( const key_type& x )
            {
                std::pair<std::wstring,Vector3> pair_array[] = { std::make_pair(std::wstring(L"Ｘ"),Vector3(1,0,0)),
                                                                 std::make_pair(std::wstring(L"Ｙ"),Vector3(0,1,0)),
                                                                 std::make_pair(std::wstring(L"Ｚ"),Vector3(0,0,1)) };
                cntl::AssocArray<std::wstring,Vector3> aa_wstr_vec3_erase_key;
                aa_wstr_vec3_erase_key.insert( pair_array, pair_array+3 );

                aa_wstr_vec3_erase_key.erase( L"Y" );
                TEST_ASSERT( aa_wstr_vec3_erase_key.size() == 3 );
                aa_wstr_vec3_erase_key.erase( L"Ｙ" );
                TEST_ASSERT( aa_wstr_vec3_erase_key.size() == 2 );
                for ( cntl::AssocArray<std::wstring,Vector3>::iterator it = aa_wstr_vec3_erase_key.begin(); it != aa_wstr_vec3_erase_key.end(); ++it )
                {
                    TEST_ASSERT( (*it).first != std::wstring(L"Ｙ") );
                }
            }

            // erase( first, last )
            {
                std::pair<std::wstring,Vector3> pair_array[] = { std::make_pair(std::wstring(L"Ｘ"),Vector3(1,0,0)),
                                                                 std::make_pair(std::wstring(L"Ｙ"),Vector3(0,1,0)),
                                                                 std::make_pair(std::wstring(L"Ｚ"),Vector3(0,0,1)) };
                cntl::AssocArray<std::wstring,Vector3> aa_wstr_vec3_erase_range;
                aa_wstr_vec3_erase_range.insert( pair_array, pair_array+3 );

                aa_wstr_vec3_erase_range.erase( aa_wstr_vec3_erase_range.begin(), aa_wstr_vec3_erase_range.end() );
                TEST_ASSERT( aa_wstr_vec3_erase_range.size() == 0 );
            }

            // swap (member function)
            {
                std::pair<std::wstring,Vector3> wstrkey_pair_array0[] = { std::make_pair(std::wstring(L"Ｘ"),Vector3(1,0,0)),
                                                                          std::make_pair(std::wstring(L"Ｙ"),Vector3(0,1,0)),
                                                                          std::make_pair(std::wstring(L"Ｚ"),Vector3(0,0,1)) };

                cntl::AssocArray<std::wstring,Vector3> aa_vec3_swap0;
                aa_vec3_swap0.insert( wstrkey_pair_array0, wstrkey_pair_array0+3 );

                std::pair<std::wstring,Vector3> wstrkey_pair_array1[] = { std::make_pair(std::wstring(L"A"),Vector3(1,1,1)),
                                                                          std::make_pair(std::wstring(L"B"),Vector3(2,2,2)),
                                                                          std::make_pair(std::wstring(L"C"),Vector3(3,3,3)),
                                                                          std::make_pair(std::wstring(L"D"),Vector3(4,4,4)) };

                cntl::AssocArray<std::wstring,Vector3> aa_vec3_swap1;
                aa_vec3_swap1.insert( wstrkey_pair_array1, wstrkey_pair_array1+4 );

                aa_vec3_swap0.swap( aa_vec3_swap1 );
                TEST_ASSERT( aa_vec3_swap0.size() == 4 );
                TEST_ASSERT( aa_vec3_swap1.size() == 3 );
                TEST_ASSERT( (*(aa_vec3_swap0.begin())).first == std::wstring(L"A") );
                TEST_ASSERT( (*(aa_vec3_swap1.begin())).first == std::wstring(L"Ｘ") );
            }

            // swap (cntl::swap)
            {
                std::pair<std::wstring,Vector3> wstrkey_pair_array0[] = { std::make_pair(std::wstring(L"Ｘ"),Vector3(1,0,0)),
                                                                          std::make_pair(std::wstring(L"Ｙ"),Vector3(0,1,0)),
                                                                          std::make_pair(std::wstring(L"Ｚ"),Vector3(0,0,1)) };

                cntl::AssocArray<std::wstring,Vector3> aa_vec3_swap0;
                aa_vec3_swap0.insert( wstrkey_pair_array0, wstrkey_pair_array0+3 );

                std::pair<std::wstring,Vector3> wstrkey_pair_array1[] = { std::make_pair(std::wstring(L"A"),Vector3(1,1,1)),
                                                                          std::make_pair(std::wstring(L"B"),Vector3(2,2,2)),
                                                                          std::make_pair(std::wstring(L"C"),Vector3(3,3,3)),
                                                                          std::make_pair(std::wstring(L"D"),Vector3(4,4,4)) };

                cntl::AssocArray<std::wstring,Vector3> aa_vec3_swap1;
                aa_vec3_swap1.insert( wstrkey_pair_array1, wstrkey_pair_array1+4 );

                cntl::swap( aa_vec3_swap0, aa_vec3_swap1 );
                TEST_ASSERT( aa_vec3_swap0.size() == 4 );
                TEST_ASSERT( aa_vec3_swap1.size() == 3 );
                TEST_ASSERT( (*(aa_vec3_swap0.begin())).first == std::wstring(L"A") );
                TEST_ASSERT( (*(aa_vec3_swap1.begin())).first == std::wstring(L"Ｘ") );
            }

            // clear
            {
                std::pair<std::wstring,Vector3> pair_array[] = { std::make_pair(std::wstring(L"Ｘ"),Vector3(1,0,0)),
                                                                 std::make_pair(std::wstring(L"Ｙ"),Vector3(0,1,0)),
                                                                 std::make_pair(std::wstring(L"Ｚ"),Vector3(0,0,1)) };

                cntl::AssocArray<std::wstring,Vector3> aa_vec3_clear;
                aa_vec3_clear.insert( pair_array, pair_array+3 );
                aa_vec3_clear.clear();
                TEST_ASSERT( aa_vec3_clear.size() == 0 );
            }

            // []
            {
                cntl::AssocArray<std::wstring,Vector3> aa_vec3_at;
                aa_vec3_at[ std::wstring(L"Ｘ") ] = Vector3(1,0,0);
                aa_vec3_at[ std::wstring(L"Ｙ") ] = Vector3(0,1,0);
                aa_vec3_at[ std::wstring(L"Ｚ") ] = Vector3(0,0,1);
                TEST_ASSERT( aa_vec3_at.size() == 3 );
                TEST_ASSERT( aa_vec3_at[ std::wstring(L"Ｘ") ] == Vector3(1,0,0) );
                TEST_ASSERT( aa_vec3_at[ std::wstring(L"Ｙ") ] == Vector3(0,1,0) );
                TEST_ASSERT( aa_vec3_at[ std::wstring(L"Ｚ") ] == Vector3(0,0,1) );
            }
        }
};

#endif
