// -*- mode: C++; coding: utf-8 -*-
#ifndef TCAAOPERATOR_H_INCLUDED
#define TCAAOPERATOR_H_INCLUDED

#include <string>
#include <cstdlib>
#include <cntl/AssocArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCAAOperator : public Test::Case
{
    const float tolerance;

public:
    TCAAOperator( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // operator ==, operator !=
            {
                std::pair<std::wstring,Vector3> wstrkey_pair_array0[] = { std::make_pair(std::wstring(L"Ｘ"),Vector3(1,0,0)),
                                                                          std::make_pair(std::wstring(L"Ｙ"),Vector3(0,1,0)),
                                                                          std::make_pair(std::wstring(L"Ｚ"),Vector3(0,0,1)) };
                cntl::AssocArray<std::wstring,Vector3> aa_vec3_op0;
                aa_vec3_op0.insert( wstrkey_pair_array0, wstrkey_pair_array0+3 );

                std::pair<std::wstring,Vector3> wstrkey_pair_array1[] = { std::make_pair(std::wstring(L"Ｘ"),Vector3(1,0,0)),
                                                                          std::make_pair(std::wstring(L"Ｙ"),Vector3(0,1,0)),
                                                                          std::make_pair(std::wstring(L"Ｚ"),Vector3(0,0,1)) };
                cntl::AssocArray<std::wstring,Vector3> aa_vec3_op1;
                aa_vec3_op1.insert( wstrkey_pair_array1, wstrkey_pair_array1+3 );

                std::pair<std::wstring,Vector3> wstrkey_pair_array2[] = { std::make_pair(std::wstring(L"A"),Vector3(1,1,1)),
                                                                          std::make_pair(std::wstring(L"B"),Vector3(2,2,2)),
                                                                          std::make_pair(std::wstring(L"C"),Vector3(3,3,3)),
                                                                          std::make_pair(std::wstring(L"D"),Vector3(4,4,4)) };
                cntl::AssocArray<std::wstring,Vector3> aa_vec3_op2;
                aa_vec3_op2.insert( wstrkey_pair_array2, wstrkey_pair_array2+4 );

                TEST_ASSERT( aa_vec3_op0 == aa_vec3_op1 );
                TEST_ASSERT( !(aa_vec3_op0 == aa_vec3_op2) );

                TEST_ASSERT( aa_vec3_op0 != aa_vec3_op2 );
                TEST_ASSERT( !(aa_vec3_op0 != aa_vec3_op1) );
            }

            // operator <, >, >=, <=
            {
                std::pair<std::wstring,Vector3> wstrkey_pair_array0[] = { std::make_pair(std::wstring(L"A"),Vector3(1,1,1)),
                                                                          std::make_pair(std::wstring(L"B"),Vector3(2,2,2)),
                                                                          std::make_pair(std::wstring(L"C"),Vector3(3,3,3)) };
                cntl::AssocArray<std::wstring,Vector3> aa_vec3_op0;
                aa_vec3_op0.insert( wstrkey_pair_array0, wstrkey_pair_array0+3 );

                std::pair<std::wstring,Vector3> wstrkey_pair_array1[] = { std::make_pair(std::wstring(L"A"),Vector3(1,1,1)),
                                                                          std::make_pair(std::wstring(L"B"),Vector3(2,2,2)),
                                                                          std::make_pair(std::wstring(L"C"),Vector3(3,3,3)),
                                                                          std::make_pair(std::wstring(L"D"),Vector3(4,4,4)) };
                cntl::AssocArray<std::wstring,Vector3> aa_vec3_op1;
                aa_vec3_op1.insert( wstrkey_pair_array1, wstrkey_pair_array1+4 );

                TEST_ASSERT( aa_vec3_op0 < aa_vec3_op1 );
                TEST_ASSERT( aa_vec3_op1 > aa_vec3_op0 );

                TEST_ASSERT( aa_vec3_op0 <= aa_vec3_op1 );
                TEST_ASSERT( aa_vec3_op1 >= aa_vec3_op0 );

                TEST_ASSERT( aa_vec3_op0 <= aa_vec3_op0 );
                TEST_ASSERT( aa_vec3_op0 >= aa_vec3_op0 );
            }
        }
};

#endif
