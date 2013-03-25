// -*- mode: C++; coding: utf-8-dos; -*-
#include <cassert>
#include <cntl/Timer.h>

#define Cntl_NameSpace_Begin namespace cntl {
#define Cntl_NameSpace_End   }

Cntl_NameSpace_Begin

Timer::Timer()
    : time_start()
    , time_now()
{
#if defined(WIN32)
    ::QueryPerformanceFrequency( &freq );
#endif
}

Timer::~Timer()
{}

#if defined(__APPLE__) || defined(__linux__)
void Timer::Reset()
{
    ::gettimeofday( &time_start, NULL );
}
#elif defined(WIN32)
void Timer::Reset()
{
    ::QueryPerformanceCounter( &time_start );
}
#endif

#if defined(__APPLE__) || defined(__linux__)
double Timer::GetElapsedTime()
{
    ::gettimeofday( &time_now, NULL );
    return GetDiff( time_now, time_start );
}
#elif defined(WIN32)
double Timer::GetElapsedTime()
{
    ::QueryPerformanceCounter( &time_now );
    return GetDiff( time_now, time_start );
}
#endif

#if defined(__APPLE__) || defined(__linux__)
double Timer::GetDiff( const timeval& end, const timeval& start )
{
    assert( (end.tv_sec > start.tv_sec) ||
            ((end.tv_sec==start.tv_sec) && end.tv_usec >= start.tv_usec) );

    int usec = end.tv_usec - start.tv_usec;
    int carry = usec < 0 ? 1 : 0;
    if ( carry )
        usec += 1000000;

    int sec = (end.tv_sec - start.tv_sec) - carry;

    return sec + double(usec) / 1000000;
}
#elif defined(WIN32)
double Timer::GetDiff( const LARGE_INTEGER& end, const LARGE_INTEGER& start )
{
    return double(end.QuadPart - start.QuadPart) / double(freq.QuadPart);
}
#endif

Cntl_NameSpace_End
