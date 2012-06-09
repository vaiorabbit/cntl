// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef MUTEX_H_INCLUDED
#define MUTEX_H_INCLUDED

#if defined(__APPLE__) || defined(__linux__)
# include "MutexPthread.h"
#elif defined(WIN32)
# include "MutexWindows.h"
#endif

namespace cntl
{
    class ScopedLock
    {
    public:

        ScopedLock( Mutex& mutex )
            : m_mutex( &mutex )
            {
                m_mutex->Lock();
            }

        ~ScopedLock()
            {
                m_mutex->Unlock();
            }

    private:
        Mutex* m_mutex;
    };
}

#endif // MUTEX_H_INCLUDED
