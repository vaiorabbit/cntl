// -*- mode: C++; coding: utf-8 -*-
#ifndef TCTIMER_H_INCLUDED
#define TCTIMER_H_INCLUDED

#include <iostream>
#include <cntl/Timer.h>
#include <TestFramework.h>

class TCTimer : public Test::Case
{
    const float tolerance;

public:
    TCTimer( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            cntl::Timer timer;
            timer.Reset();

            int nTimerReset = 10;
            int nCheckElapsedTime = 1000;
            for ( int iTimerReset = 0; iTimerReset < nTimerReset; ++iTimerReset )
            {
                timer.Reset();
                for ( int iCheckElapsedTime = 0; iCheckElapsedTime < nCheckElapsedTime; ++iCheckElapsedTime )
                {
                    std::cout
                        << "iTimerReset=" << iTimerReset << ", "
                        << "iCheckElapsedTime=" << iCheckElapsedTime << " : "
                        << "GetElapsedTime=" << timer.GetElapsedTime() << std::endl;
                }
            }
        }
};

#endif
