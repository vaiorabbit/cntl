// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef CNTL_EVENTPTHREAD_H_INCLUDED
#define CNTL_EVENTPTHREAD_H_INCLUDED

#include <pthread.h>

namespace cntl
{
    //
    // Win32 Event Object Wrapper.
    //

    class Event
    {
    public:

        Event( bool manual_reset_ = true, bool initial_state_ = false )
            : manual_reset( manual_reset_ )
            , signal_ready( initial_state_ )
            {
                pthread_cond_init( &signal_cond, NULL );
                pthread_mutex_init( &signal_mutex, NULL );
            }

        ~Event()
            {
                pthread_cond_destroy( &signal_cond );
                pthread_mutex_destroy( &signal_mutex );
            }

        void Set()
            {
                pthread_mutex_lock( &signal_mutex );

                if ( !signal_ready )
                {
                    signal_ready = true;
                    pthread_cond_signal( &signal_cond );
                }

                pthread_mutex_unlock( &signal_mutex );
            }

        void Reset()
            {
                pthread_mutex_lock( &signal_mutex );

                signal_ready = false;

                pthread_mutex_unlock( &signal_mutex );
            }

        void Wait()
            {
                pthread_mutex_lock( &signal_mutex );

                while ( !signal_ready )
                {
                    pthread_cond_wait( &signal_cond, &signal_mutex );
                }

                if ( !manual_reset )
                    signal_ready = false;

                pthread_mutex_unlock( &signal_mutex );
            }

    private:

        bool manual_reset;
        bool signal_ready;
        pthread_cond_t signal_cond;
        pthread_mutex_t signal_mutex;
    };

}

#endif // CNTL_EVENTPTHREAD_H_INCLUDED
