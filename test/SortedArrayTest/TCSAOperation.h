// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSAOPERATION_H_INCLUDED
#define TCSAOPERATION_H_INCLUDED

#include <string>
#include <cstdlib>
#include <cntl/SortedArray.h>
#include <TestFramework.h>
#include "Vector3.h"

class TCSAOperation : public Test::Case
{
    const float tolerance;

public:
    TCSAOperation( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // key_comp()
            {
                std::wstring pair_array[] = { std::wstring(L"Ｘ"),
                                              std::wstring(L"Ｙ"),
                                              std::wstring(L"Ｚ") };
                cntl::SortedArray<std::wstring> sa_wstr_key_comp;
                sa_wstr_key_comp.insert( pair_array, pair_array+3 );

                cntl::SortedArray<std::wstring>::key_compare fn = sa_wstr_key_comp.key_comp();
                TEST_ASSERT( fn(std::wstring(L"Ｘ"), std::wstring(L"Ｙ")) );
            }

            // value_comp()
            {
                std::wstring wstrkey_pair_array[] = { std::wstring(L"A"),
                                                      std::wstring(L"B"),
                                                      std::wstring(L"C"),
                                                      std::wstring(L"D") };

                cntl::SortedArray<std::wstring> sa_wstr_value_comp;
                sa_wstr_value_comp.insert( wstrkey_pair_array, wstrkey_pair_array+4 );

                cntl::SortedArray<std::wstring>::value_compare fn = sa_wstr_value_comp.value_comp();
                TEST_ASSERT( fn( wstrkey_pair_array[0], wstrkey_pair_array[1] ) );
            }

            // get_allocator()
            {
                cntl::SortedArray<Vector3> sa_wstr_alloc;
                cntl::SortedArray<Vector3>::allocator_type ma = sa_wstr_alloc.get_allocator();
                cntl::SortedArray<Vector3>::value_type* area = (cntl::SortedArray<Vector3>::value_type*)ma.allocate( 65536 );
                TEST_ASSERT( area != NULL );
                ma.deallocate( area, 65536 );
            }

            // find( const key_type& x )
            {
                std::wstring wstrkey_pair_array[] = { std::wstring(L"A"),
                                                      std::wstring(L"B"),
                                                      std::wstring(L"C"),
                                                      std::wstring(L"D") };

                cntl::SortedArray<std::wstring> sa_wstr_find;
                sa_wstr_find.insert( wstrkey_pair_array, wstrkey_pair_array+4 );

                cntl::SortedArray<std::wstring>::const_iterator it = sa_wstr_find.find( std::wstring(L"B") );
                TEST_ASSERT( it == sa_wstr_find.begin()+1 );
            }

            // count( const key_type& x )
            {
                std::wstring wstrkey_pair_array[] = { std::wstring(L"A"),
                                                      std::wstring(L"B"),
                                                      std::wstring(L"C"),
                                                      std::wstring(L"D") };

                cntl::SortedArray<std::wstring> sa_wstr_count;
                sa_wstr_count.insert( wstrkey_pair_array, wstrkey_pair_array+4 );
                TEST_ASSERT( sa_wstr_count.count( std::wstring(L"B") ) == 1 );
            }

            // lower_bound( const key_type& x )
            // upper_bound( const key_type& x )
            {
                // Ref.: http://www.cplusplus.com/reference/stl/map/lower_bound/
                cntl::SortedArray<int> aa_char_int;
                cntl::SortedArray<int>::iterator itlow, itup;

                aa_char_int.insert( 20 );
                aa_char_int.insert( 40 );
                aa_char_int.insert( 60 );
                aa_char_int.insert( 80 );
                aa_char_int.insert( 100 );

                itlow = aa_char_int.lower_bound ( 40 ); // itlow points to 20
                itup  = aa_char_int.upper_bound ( 80 ); // itup points to 100 (not 80!)

                aa_char_int.erase( itlow, itup );      // erases [itlow,itup)
                TEST_ASSERT( aa_char_int.size() == 2 );
                TEST_ASSERT( (*(aa_char_int.begin())) == 20 );
                TEST_ASSERT( (*(aa_char_int.end()-1)) == 100 );
            }

            // equal_range( const key_type& x )
            {
                // Ref.: http://www.cplusplus.com/reference/stl/map/equal_range/
                cntl::SortedArray<int> aa_char_int;
                aa_char_int.insert( 20 );
                aa_char_int.insert( 40 );
                aa_char_int.insert( 60 );

                std::pair< cntl::SortedArray<int>::iterator, cntl::SortedArray<int>::iterator >
                    ret = aa_char_int.equal_range( 40 );

                cntl::SortedArray<int>::iterator itlow, itup;
                itlow = aa_char_int.lower_bound (40);
                itup  = aa_char_int.upper_bound (40);

                TEST_ASSERT( ret.first == itlow );
                TEST_ASSERT( ret.second == itup );
            }
        }
};

#endif
