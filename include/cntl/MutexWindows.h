// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef MUTEX_WINDOWS_H_INCLUDED
#define MUTEX_WINDOWS_H_INCLUDED

#include <windows.h>

namespace cntl
{
    class Mutex
    {
    public:

        Mutex()
            {
                ::InitializeCriticalSection( &m_cs );
            }

        ~Mutex()
            {
                ::DeleteCriticalSection( &m_cs );
            }

        void Lock()
            {
                ::EnterCriticalSection( &m_cs );
            }

        void Unlock()
            {
                ::LeaveCriticalSection( &m_cs );
            }

    private:

        CRITICAL_SECTION m_cs;
    };
}

#endif // MUTEX_WINDOWS_H_INCLUDED
