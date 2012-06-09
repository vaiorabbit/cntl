// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef MUTEX_PTHREAD_H_INCLUDED
#define MUTEX_PTHREAD_H_INCLUDED

#include <pthread.h>

namespace cntl
{
    class Mutex
    {
    public:

        Mutex()
            {
                pthread_mutex_init( &m_mutex, NULL );
            }

        ~Mutex()
            {
                pthread_mutex_destroy( &m_mutex );
            }

        void Lock()
            {
                pthread_mutex_lock( &m_mutex );
            }

        void Unlock()
            {
                pthread_mutex_unlock( &m_mutex );
            }

    private:

        pthread_mutex_t m_mutex;
    };
}

#endif // MUTEX_PTHREAD_H_INCLUDED
