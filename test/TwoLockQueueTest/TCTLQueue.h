// -*- mode: C++; coding: utf-8 -*-
#ifndef TCDAACCESSOR_H_INCLUDED
#define TCDAACCESSOR_H_INCLUDED

#include <cntl/TwoLockQueue.h>
#include <cntl/String.h>
#include <TestFramework.h>

class TCTLQueue : public Test::Case
{
    const float tolerance;

public:
    TCTLQueue( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            // front / back
            cntl::TwoLockQueue<int> tlq;
            tlq.Initialize();
            for ( int i = 0; i < 3; ++i )
                tlq.Enqueue( i+1 );

            int out = 0;
            bool result = false;

            result = tlq.Dequeue( &out );
            TEST_ASSERT_EQUAL( out, 1 );
            TEST_ASSERT_EQUAL( result, true );

            result = tlq.Dequeue( &out );
            TEST_ASSERT_EQUAL( out, 2 );
            TEST_ASSERT_EQUAL( result, true );

            result = tlq.Dequeue( &out );
            TEST_ASSERT_EQUAL( out, 3 );
            TEST_ASSERT_EQUAL( result, true );

            result = tlq.Dequeue( &out );
            TEST_ASSERT_EQUAL( result, false );

            tlq.Finalize();
        }
};

#endif
