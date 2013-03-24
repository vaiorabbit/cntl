// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef CNTL_EVENTWINDOWS_H_INCLUDED
#define CNTL_EVENTWINDOWS_H_INCLUDED

#include <windows.h>

namespace cntl
{
    //
    // Win32 Event Object Wrapper.
    //

    class Event
    {
    public:

        Event( bool manual_reset_ = true, bool initial_state_ = false )
            : m_hEvent(INVALID_HANDLE_VALUE)
            {
                m_hEvent = ::CreateEvent( NULL, manual_reset_, initial_state_, "" );
            }

        ~Event()
            {
                ::CloseHandle( m_hEvent );
            }

        void Set()
            {
                ::SetEvent( m_hEvent );
            }

        void Reset()
            {
                ::ResetEvent( m_hEvent );
            }

        void Wait()
            {
                ::WaitForSingleObject( m_hEvent, INFINITE );
            }

    private:

        HANDLE m_hEvent;
    };

}

#endif // CNTL_EVENTWINDOWS_H_INCLUDED
