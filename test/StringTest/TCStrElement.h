// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSTRELEMENT_H_INCLUDED
#define TCSTRELEMENT_H_INCLUDED

#include <cntl/Memory.h>
#include <cntl/String.h>
#include <TestFramework.h>

class TCStrElement : public Test::Case
{
public:
    TCStrElement( const char* name )
        : Test::Case( name )
        {}

    ~TCStrElement()
        {}

    virtual void Run()
        {
            char src[] = "C String";
            cntl::String str_fromcstr( src );
            for ( std::size_t i = 0; i < str_fromcstr.size(); ++i )
            {
                TEST_ASSERT( str_fromcstr[i] == src[i] );
            }
        }
};

#endif
