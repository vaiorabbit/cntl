// -*- mode: C++; coding: utf-8 -*-
#ifndef TCAAOPERATION_H_INCLUDED
#define TCAAOPERATION_H_INCLUDED

#include <string>
#include <cstdlib>
#include <cntl/AssocArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCAAOperation : public Test::Case
{
    const float tolerance;

public:
    TCAAOperation( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // key_comp()
            {
                std::pair<std::wstring,Vector3> pair_array[] = { std::make_pair(std::wstring(L"Ｘ"),Vector3(1,0,0)),
                                                                 std::make_pair(std::wstring(L"Ｙ"),Vector3(0,1,0)),
                                                                 std::make_pair(std::wstring(L"Ｚ"),Vector3(0,0,1)) };
                cntl::AssocArray<std::wstring,Vector3> aa_wstr_vec3_key_comp;
                aa_wstr_vec3_key_comp.insert( pair_array, pair_array+3 );

                cntl::AssocArray<std::wstring,Vector3>::key_compare fn = aa_wstr_vec3_key_comp.key_comp();
                TEST_ASSERT( fn(std::wstring(L"Ｘ"), std::wstring(L"Ｙ")) );
            }

            // value_comp()
            {
                std::pair<std::wstring,Vector3> wstrkey_pair_array[] = { std::make_pair(std::wstring(L"A"),Vector3(1,1,1)),
                                                                         std::make_pair(std::wstring(L"B"),Vector3(2,2,2)),
                                                                         std::make_pair(std::wstring(L"C"),Vector3(3,3,3)),
                                                                         std::make_pair(std::wstring(L"D"),Vector3(4,4,4)) };

                cntl::AssocArray<std::wstring,Vector3> aa_wstr_vec3_value_comp;
                aa_wstr_vec3_value_comp.insert( wstrkey_pair_array, wstrkey_pair_array+4 );

                cntl::AssocArray<std::wstring,Vector3>::value_compare fn = aa_wstr_vec3_value_comp.value_comp();
                TEST_ASSERT( fn( wstrkey_pair_array[0], wstrkey_pair_array[1] ) );
            }

            // get_allocator()
            {
                cntl::AssocArray<std::wstring,Vector3> aa_wstr_vec3_alloc;
                cntl::AssocArray<std::wstring,Vector3>::allocator_type ma = aa_wstr_vec3_alloc.get_allocator();
                cntl::AssocArray<std::wstring,Vector3>::value_type* area = (cntl::AssocArray<std::wstring,Vector3>::value_type*)ma.allocate( 65536 );
                TEST_ASSERT( area != NULL );
                ma.deallocate( area, 65536 );
            }

            // find( const key_type& x )
            {
                std::pair<std::wstring,Vector3> wstrkey_pair_array[] = { std::make_pair(std::wstring(L"A"),Vector3(1,1,1)),
                                                                         std::make_pair(std::wstring(L"B"),Vector3(2,2,2)),
                                                                         std::make_pair(std::wstring(L"C"),Vector3(3,3,3)),
                                                                         std::make_pair(std::wstring(L"D"),Vector3(4,4,4)) };

                cntl::AssocArray<std::wstring,Vector3> aa_wstr_vec3_find;
                aa_wstr_vec3_find.insert( wstrkey_pair_array, wstrkey_pair_array+4 );

                cntl::AssocArray<std::wstring,Vector3>::const_iterator it = aa_wstr_vec3_find.find( std::wstring(L"B") );
                TEST_ASSERT( it == aa_wstr_vec3_find.begin()+1 );
            }

            // count( const key_type& x )
            {
                std::pair<std::wstring,Vector3> wstrkey_pair_array[] = { std::make_pair(std::wstring(L"A"),Vector3(1,1,1)),
                                                                         std::make_pair(std::wstring(L"B"),Vector3(2,2,2)),
                                                                         std::make_pair(std::wstring(L"C"),Vector3(3,3,3)),
                                                                         std::make_pair(std::wstring(L"D"),Vector3(4,4,4)) };

                cntl::AssocArray<std::wstring,Vector3> aa_wstr_vec3_count;
                aa_wstr_vec3_count.insert( wstrkey_pair_array, wstrkey_pair_array+4 );
                TEST_ASSERT( aa_wstr_vec3_count.count( std::wstring(L"B") ) == 1 );
            }

            // lower_bound( const key_type& x )
            // upper_bound( const key_type& x )
            {
                // Ref.: http://www.cplusplus.com/reference/stl/map/lower_bound/
                cntl::AssocArray<char,int> aa_char_int;
                cntl::AssocArray<char,int>::iterator itlow, itup;

                aa_char_int['a'] = 20;
                aa_char_int['b'] = 40;
                aa_char_int['c'] = 60;
                aa_char_int['d'] = 80;
                aa_char_int['e'] = 100;

                itlow = aa_char_int.lower_bound ('b'); // itlow points to b
                itup  = aa_char_int.upper_bound ('d'); // itup points to e (not d!)

                aa_char_int.erase( itlow, itup );      // erases [itlow,itup)
                TEST_ASSERT( aa_char_int.size() == 2 );
                TEST_ASSERT( (*(aa_char_int.begin())).second == 20 );
                TEST_ASSERT( (*(aa_char_int.end()-1)).second == 100 );
            }

            // equal_range( const key_type& x )
            {
                // Ref.: http://www.cplusplus.com/reference/stl/map/equal_range/
                cntl::AssocArray<char,int> aa_char_int;
                aa_char_int['a'] = 20;
                aa_char_int['b'] = 40;
                aa_char_int['c'] = 60;

                std::pair< cntl::AssocArray<char,int>::iterator, cntl::AssocArray<char,int>::iterator >
                    ret = aa_char_int.equal_range( 'b' );

                cntl::AssocArray<char,int>::iterator itlow, itup;
                itlow = aa_char_int.lower_bound ('b');
                itup  = aa_char_int.upper_bound ('b');

                TEST_ASSERT( ret.first == itlow );
                TEST_ASSERT( ret.second == itup );
            }
        }
};

#endif
