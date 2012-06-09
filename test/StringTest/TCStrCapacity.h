// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSTRCAPACITY_H_INCLUDED
#define TCSTRCAPACITY_H_INCLUDED

#include <cntl/Memory.h>
#include <cntl/String.h>
#include <TestFramework.h>

class TCStrCapacity : public Test::Case
{
public:
    TCStrCapacity( const char* name )
        : Test::Case( name )
        {}

    ~TCStrCapacity()
        {}

    virtual void Run()
        {
            cntl::String str_default;
            TEST_ASSERT( str_default.size() == 0 );
            TEST_ASSERT( str_default.empty() == true );
            str_default.clear();
            TEST_ASSERT( str_default.size() == 0 );
            TEST_ASSERT( str_default.empty() == true );

            cntl::String str_fromcstr( "C String" );
            TEST_ASSERT( str_fromcstr.size() == 8 );
            TEST_ASSERT( str_fromcstr.empty() == false );
            str_fromcstr.clear();
            TEST_ASSERT( str_fromcstr.size() == 0 );
            TEST_ASSERT( str_fromcstr.empty() == true );
        }
};

#endif
