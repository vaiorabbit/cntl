// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef CNTL_TIMER_H_INCLUDED
#define CNTL_TIMER_H_INCLUDED

#if defined(__APPLE__) || defined(__linux__)
# include <sys/time.h> // timeval, gettimeofday
#elif defined(WIN32)
# include <windows.h> // timeval, gettimeofday
#endif

namespace cntl
{
    class Timer
    {
    public:
        Timer();
        ~Timer();

        void Reset();
        double GetElapsedTime();

    private:
#if defined(__APPLE__) || defined(__linux__)
        timeval time_start, time_now;
        double GetDiff( const timeval& end, const timeval& start );
#elif defined(WIN32)
        LARGE_INTEGER time_start, time_now, freq;
        double GetDiff( const LARGE_INTEGER& end, const LARGE_INTEGER& start );
#endif
    };
}
#endif // CNTL_TIMER_H_INCLUDED
