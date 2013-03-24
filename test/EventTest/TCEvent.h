// -*- mode: C++; coding: utf-8 -*-
#ifndef TCEVENT_H_INCLUDED
#define TCEVENT_H_INCLUDED

#include <iostream>
#include <cntl/Event.h>
#include <cntl/Thread.h>
#include <cntl/Timer.h>
#include <TestFramework.h>

static cntl::Event gs_Thread0GoEvent;
static cntl::Event gs_Thread1GoEvent;

class TCEvent : public Test::Case
{
    const float tolerance;

    class TestThread0 : public cntl::Thread
    {
    public:
        void Run()
            {
                m_count = 0;

                while ( Suspended() )
                    WaitForResume();

                while ( Runnable() )
                {
                    gs_Thread0GoEvent.Wait();

                    std::cout << "TestEvent[0]" << std::endl;

                    gs_Thread1GoEvent.Set();
                    gs_Thread0GoEvent.Reset();

                    if ( ++m_count >= 10 )
                        Terminate();
                }
            }

    private:
        int m_count;
    };

    class TestThread1 : public cntl::Thread
    {
    public:
        void Run()
            {
                m_count = 0;

                while ( Suspended() )
                    WaitForResume();

                while ( Runnable() )
                {
                    gs_Thread1GoEvent.Wait();

                    std::cout << "TestEvent[1]" << std::endl;

                    gs_Thread0GoEvent.Set();
                    gs_Thread1GoEvent.Reset();

                    if ( ++m_count >= 10 )
                        Terminate();
                }
            }

    private:
        int m_count;
    };

    TestThread0 m_thread0;
    TestThread1 m_thread1;

public:
    TCEvent( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            gs_Thread0GoEvent.Set();
            gs_Thread1GoEvent.Reset();

            m_thread0.Create(0, true);
            m_thread1.Create(0, true);

            m_thread0.Resume();
            m_thread1.Resume();

            m_thread0.Join();
            m_thread1.Join();
        }
};

#endif
