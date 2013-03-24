// -*- mode: C++; coding: utf-8 -*-
#ifndef TCTHREAD_H_INCLUDED
#define TCTHREAD_H_INCLUDED

#include <iostream>
#include <cntl/Thread.h>
#include <cntl/Timer.h>
#include <TestFramework.h>

class TCThread : public Test::Case
{
    const float tolerance;

    class TestThread : public cntl::Thread
    {
    public:
        void Run()
            {
                while ( Suspended() )
                    WaitForResume();

                while ( Runnable() )
                {
                    std::cout << "TestThread[" << m_id << "]" << std::endl;
                }
            }

        TestThread()
            : cntl::Thread()
            , m_id(-1)
            {}

        TestThread( int id )
            : m_id(id)
            {}

        void SetID( int id )
            { m_id = id; }

    private:
        int m_id;
    };

    TestThread m_thread[2];

public:
    TCThread( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            for ( int i = 0; i < 2; ++i )
            {
                m_thread[i].Create(0, true);
                m_thread[i].SetID( i );
            }

            cntl::Timer timer;
            for ( int i = 0; i < 2; ++i )
            {
                m_thread[i].Resume();
            }

            timer.Reset();
            while ( timer.GetElapsedTime() < 1.0 )
            {
                std::cout << timer.GetElapsedTime() << std::endl;
            }

            for ( int i = 0; i < 2; ++i )
            {
                m_thread[i].Terminate();
            }
        }
};

#endif
