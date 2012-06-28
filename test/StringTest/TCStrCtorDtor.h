// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSTRCTORDOTR_H_INCLUDED
#define TCSTRCTORDOTR_H_INCLUDED

#include <cntl/Memory.h>
#include <cntl/String.h>
#include <TestFramework.h>

class TCStrCtorDtor : public Test::Case
{
public:
    TCStrCtorDtor( const char* name )
        : Test::Case( name )
        {}

    ~TCStrCtorDtor()
        {}

    virtual void Run()
        {
            cntl::String str_default;
            TEST_ASSERT( std::strcmp( str_default.c_str(), "" ) == 0 );
            // TEST_ASSERT( str_default.c_str() == NULL );

            cntl::String str_fromcstr( "C String" );
            TEST_ASSERT( std::strcmp( str_fromcstr.c_str(), "C String" ) == 0 );

            cntl::String str_copied( str_fromcstr );
            TEST_ASSERT( std::strcmp( str_copied.c_str(), "C String" ) == 0 );

            cntl::String str_assignop_string;
            str_assignop_string = cntl::String( "Assign Operator" );
            TEST_ASSERT( std::strcmp( str_assignop_string.c_str(), "Assign Operator" ) == 0 );

            cntl::String str_assignop_cstr;
            str_assignop_cstr = "Assign Operator";
            TEST_ASSERT( std::strcmp( str_assignop_cstr.c_str(), "Assign Operator" ) == 0 );
        }
};

#endif
