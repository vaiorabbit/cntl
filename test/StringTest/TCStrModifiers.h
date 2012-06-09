// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSTRMODIFIERS_H_INCLUDED
#define TCSTRMODIFIERS_H_INCLUDED

#include <cntl/Memory.h>
#include <cntl/String.h>
#include <TestFramework.h>

class TCStrModifiers : public Test::Case
{
public:
    TCStrModifiers( const char* name )
        : Test::Case( name )
        {}

    ~TCStrModifiers()
        {}

    virtual void Run()
        {
            // operator +=

            {
                cntl::String str_append_cstr( "Appending " );
                str_append_cstr += "C String";
                TEST_ASSERT( str_append_cstr == "Appending C String" );
            }

            {
                cntl::String str_append_string( "Appending " );
                str_append_string += cntl::String( "cntl::String" );
                TEST_ASSERT( str_append_string == "Appending cntl::String" );
            }

            // erase

            {
                cntl::String str_erased( "Yet another C string wrapper." );
                str_erased.erase();
                TEST_ASSERT( str_erased.size() == 0 );
                TEST_ASSERT( str_erased == "" );
            }

            {
                cntl::String str_erased( "Yet another C string wrapper." );
                str_erased.erase( 11 );
                TEST_ASSERT( str_erased == "Yet another" );
            }

            {
                cntl::String str_erased( "Yet another C string wrapper." );
                str_erased.erase( 11, 9 );
                TEST_ASSERT( str_erased == "Yet another wrapper." );
            }

            // swap
            {
                cntl::String str0("String #0"), str1("String #1");
                str0.swap( str1 );
                TEST_ASSERT( str0 == "String #1" );
                TEST_ASSERT( str1 == "String #0" );
            }
        }
};

#endif
