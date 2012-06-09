// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSAOPERATOR_H_INCLUDED
#define TCSAOPERATOR_H_INCLUDED

#include <string>
#include <cstdlib>
#include <cntl/SortedArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCSAOperator : public Test::Case
{
    const float tolerance;

public:
    TCSAOperator( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // operator ==, operator !=
            {
                std::wstring wstrkey_pair_array0[] = { std::wstring(L"Ｘ"),
                                                       std::wstring(L"Ｙ"),
                                                       std::wstring(L"Ｚ") };
                cntl::SortedArray<std::wstring> sa_wstr_op0;
                sa_wstr_op0.insert( wstrkey_pair_array0, wstrkey_pair_array0+3 );

                std::wstring wstrkey_pair_array1[] = { std::wstring(L"Ｘ"),
                                                       std::wstring(L"Ｙ"),
                                                       std::wstring(L"Ｚ") };
                cntl::SortedArray<std::wstring> sa_wstr_op1;
                sa_wstr_op1.insert( wstrkey_pair_array1, wstrkey_pair_array1+3 );

                std::wstring wstrkey_pair_array2[] = { std::wstring(L"A"),
                                                       std::wstring(L"B"),
                                                       std::wstring(L"C"),
                                                       std::wstring(L"D"), };
                cntl::SortedArray<std::wstring> sa_wstr_op2;
                sa_wstr_op2.insert( wstrkey_pair_array2, wstrkey_pair_array2+4 );

                TEST_ASSERT( sa_wstr_op0 == sa_wstr_op1 );
                TEST_ASSERT( !(sa_wstr_op0 == sa_wstr_op2) );

                TEST_ASSERT( sa_wstr_op0 != sa_wstr_op2 );
                TEST_ASSERT( !(sa_wstr_op0 != sa_wstr_op1) );
            }

            // operator <, >, >=, <=
            {
                std::wstring wstrkey_pair_array0[] = { std::wstring(L"A"),
                                                       std::wstring(L"B"),
                                                       std::wstring(L"C"), };
                cntl::SortedArray<std::wstring> sa_wstr_op0;
                sa_wstr_op0.insert( wstrkey_pair_array0, wstrkey_pair_array0+3 );

                std::wstring wstrkey_pair_array1[] = { std::wstring(L"A"),
                                                       std::wstring(L"B"),
                                                       std::wstring(L"C"),
                                                       std::wstring(L"D") };
                cntl::SortedArray<std::wstring> sa_wstr_op1;
                sa_wstr_op1.insert( wstrkey_pair_array1, wstrkey_pair_array1+4 );

                TEST_ASSERT( sa_wstr_op0 < sa_wstr_op1 );
                TEST_ASSERT( sa_wstr_op1 > sa_wstr_op0 );

                TEST_ASSERT( sa_wstr_op0 <= sa_wstr_op1 );
                TEST_ASSERT( sa_wstr_op1 >= sa_wstr_op0 );

                TEST_ASSERT( sa_wstr_op0 <= sa_wstr_op0 );
                TEST_ASSERT( sa_wstr_op0 >= sa_wstr_op0 );
            }
        }
};

#endif
