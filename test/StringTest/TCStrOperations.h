// -*- mode: C++; coding: utf-8 -*-
#ifndef TCSTROPERATIONS_H_INCLUDED
#define TCSTROPERATIONS_H_INCLUDED

#include <cntl/Memory.h>
#include <cntl/String.h>
#include <TestFramework.h>

class TCStrOperations : public Test::Case
{
public:
    TCStrOperations( const char* name )
        : Test::Case( name )
        {}

    ~TCStrOperations()
        {}

    virtual void Run()
        {
            // substr
            {
                cntl::String str_src( "Yet another C string wrapper." );

                cntl::String str_dst( str_src.substr() );
                TEST_ASSERT( str_dst.size() == str_src.size() );
                TEST_ASSERT( str_dst == str_src );
            }

            {
                cntl::String str_src( "Yet another C string wrapper." );

                cntl::String str_dst( str_src.substr( 12 ) );
                TEST_ASSERT( str_dst.size() == 17 );
                TEST_ASSERT( str_dst == "C string wrapper." );
            }

            {
                cntl::String str_src( "Yet another C string wrapper." );

                cntl::String str_dst( str_src.substr( 12, 8 ) );
                TEST_ASSERT( str_dst.size() == 8 );
                TEST_ASSERT( str_dst == "C string" );
            }

            // find

            {
                cntl::String str_src( "Yet another C string wrapper." );
                std::size_t p;

                p = str_src.find( cntl::String("another") );
                TEST_ASSERT( p == 4 );

                p = str_src.find( cntl::String("another"), 4 );
                TEST_ASSERT( p == 4 );

                p = str_src.find( cntl::String("one another") );
                TEST_ASSERT( p == cntl::String::npos );
            }

            {
                cntl::String str_src( "Yet another C string wrapper." );
                std::size_t p;

                p = str_src.find( "another" );
                TEST_ASSERT( p == 4 );

                p = str_src.find( "another", 4 );
                TEST_ASSERT( p == 4 );

                p = str_src.find( "one another" );
                TEST_ASSERT( p == cntl::String::npos );
            }

            {
                cntl::String str_src( "Yet another C string wrapper." );
                std::size_t p;

                p = str_src.find( 'a' );
                TEST_ASSERT( p == 4 );

                p = str_src.find( 'a', 6 );
                TEST_ASSERT( p == 23 );

                p = str_src.find( 'x' );
                TEST_ASSERT( p == cntl::String::npos );
            }

            // find_first_of

            {
                cntl::String str_src( "Yet another C string wrapper." );
                std::size_t p;

                p = str_src.find_first_of( cntl::String("aeiou") );
                TEST_ASSERT( p == 1 );

                p = str_src.find_first_of( cntl::String("aeiou"), 4 );
                TEST_ASSERT( p == 4 );

                p = str_src.find_first_of( cntl::String("xyz") );
                TEST_ASSERT( p == cntl::String::npos );
            }

            {
                cntl::String str_src( "Yet another C string wrapper." );
                std::size_t p;

                p = str_src.find_first_of( "aeiou" );
                TEST_ASSERT( p == 1 );

                p = str_src.find_first_of( "aeiou", 4 );
                TEST_ASSERT( p == 4 );

                p = str_src.find_first_of( "xyz" );
                TEST_ASSERT( p == cntl::String::npos );
            }

            {
                cntl::String str_src( "Yet another C string wrapper." );
                std::size_t p;

                p = str_src.find_first_of( 'e' );
                TEST_ASSERT( p == 1 );

                p = str_src.find_first_of( 'a', 4 );
                TEST_ASSERT( p == 4 );

                p = str_src.find_first_of( 'x' );
                TEST_ASSERT( p == cntl::String::npos );
            }

            // find_first_not_of

            {
                cntl::String str_src( "Yet another C string wrapper." );
                std::size_t p;

                p = str_src.find_first_not_of( cntl::String("Y") );
                TEST_ASSERT( p == 1 );

                p = str_src.find_first_not_of( cntl::String("aeiou"), 4 );
                TEST_ASSERT( p == 5 );

                p = str_src.find_first_not_of( cntl::String(". aCeghinoprstwY") );
                TEST_ASSERT( p == cntl::String::npos );
            }

            {
                cntl::String str_src( "Yet another C string wrapper." );
                std::size_t p;

                p = str_src.find_first_not_of( "Y" );
                TEST_ASSERT( p == 1 );

                p = str_src.find_first_not_of( "aeiou", 4 );
                TEST_ASSERT( p == 5 );

                p = str_src.find_first_not_of( ". aCeghinoprstwY" );
                TEST_ASSERT( p == cntl::String::npos );
            }

            {
                cntl::String str_src( "Yet another C string wrapper." );
                std::size_t p;

                p = str_src.find_first_not_of( 'Y' );
                TEST_ASSERT( p == 1 );

                p = str_src.find_first_not_of( 'a', 4 );
                TEST_ASSERT( p == 5 );
            }
        }
};

#endif
