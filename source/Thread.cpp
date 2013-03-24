#include <cassert>
#include <cntl/Thread.h>

#define Cntl_NameSpace_Begin namespace cntl {
#define Cntl_NameSpace_End   }

Cntl_NameSpace_Begin

Thread::Thread()
    : m_hThread()
    , m_mutexRunnable()
    , m_bRunnable(true)
#if defined(__APPLE__) || defined(__linux__)
    , m_mutexSuspend()
    , m_condResume()
    , m_bSuspended(false)
#elif defined(WIN32)
#endif // defined(__APPLE__) || defined(__linux__)
{}

Thread::~Thread()
{}

bool
Thread::Create( unsigned int nStackSize /*= 0*/, bool bCreateSuspended /*= false*/  )
{
#if defined(__APPLE__) || defined(__linux__)
    // Suspend/Resume
    pthread_mutex_init( &m_mutexSuspend, NULL );
    pthread_cond_init( &m_condResume, NULL );
    m_bSuspended = bCreateSuspended;

    // Runnable
    pthread_mutex_init( &m_mutexRunnable, NULL );
    m_bRunnable = true;

    // Thread
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    // if ( nStackSize > PTHREAD_STACK_MIN )
    if ( nStackSize > 0 )
    {
        pthread_attr_setstacksize( &attr, nStackSize );
    }
    pthread_create( &m_hThread, &attr, BootThread, this );
    pthread_attr_destroy( &attr );
#endif // defined(__APPLE__) || defined(__linux__)

    return true;
}

void
Thread::Terminate()
{
#if defined(__APPLE__) || defined(__linux__)
    if ( m_bRunnable )
    {
        pthread_mutex_lock( &m_mutexRunnable );
        if ( m_bRunnable )
        {
            m_bRunnable = false;
            Resume();
        }
        pthread_mutex_unlock( &m_mutexRunnable );
    }
#endif // defined(__APPLE__) || defined(__linux__)
}

bool
Thread::Runnable()
{
#if defined(__APPLE__) || defined(__linux__)
    pthread_mutex_lock( &m_mutexRunnable );
    bool bRunnable = m_bRunnable;
    pthread_mutex_unlock( &m_mutexRunnable );
    return bRunnable;
#endif // defined(__APPLE__) || defined(__linux__)
}

void
Thread::Join()
{
#if defined(__APPLE__) || defined(__linux__)
    pthread_join( m_hThread, NULL );
#endif // defined(__APPLE__) || defined(__linux__)
}

void
Thread::Exit()
{
#if defined(__APPLE__) || defined(__linux__)
    pthread_exit( NULL );
#endif // defined(__APPLE__) || defined(__linux__)
}

bool
Thread::Suspended()
{
#if defined(__APPLE__) || defined(__linux__)
    pthread_mutex_lock( &m_mutexSuspend );
    bool bSuspended = m_bSuspended;
    pthread_mutex_unlock( &m_mutexSuspend );
    return bSuspended;
#endif // defined(__APPLE__) || defined(__linux__)
}

void
Thread::Suspend()
{
#if defined(__APPLE__) || defined(__linux__)
    if ( !m_bSuspended )
    {
        pthread_mutex_lock( &m_mutexSuspend );
        if ( !m_bSuspended )
        {
            m_bSuspended = true;
            do { // gurad for spurious wakeup
                pthread_cond_wait( &m_condResume, &m_mutexSuspend );
            } while ( m_bSuspended );
        }
        pthread_mutex_unlock( &m_mutexSuspend );
    }
#endif // defined(__APPLE__) || defined(__linux__)
}

void
Thread::Resume()
{
#if defined(__APPLE__) || defined(__linux__)
    if ( m_bSuspended )
    {
        pthread_mutex_lock( &m_mutexSuspend );
        if ( m_bSuspended )
        {
            m_bSuspended = false;
            pthread_cond_signal( &m_condResume );
        }
        pthread_mutex_unlock( &m_mutexSuspend );
    }
#endif // defined(__APPLE__) || defined(__linux__)
}

void
Thread::WaitForResume()
{
#if defined(__APPLE__) || defined(__linux__)
    pthread_mutex_lock( &m_mutexSuspend );
    while ( m_bSuspended ) { // gurad for spurious wakeup
        pthread_cond_wait( &m_condResume, &m_mutexSuspend );
    }
    pthread_mutex_unlock( &m_mutexSuspend );
#endif // defined(__APPLE__) || defined(__linux__)
}

// static
#if defined(__APPLE__) || defined(__linux__)
void* Thread::BootThread( void* pThread )
#elif defined(WIN32)
DWORD __stdcall BootThread( LPVOID pThread )
#endif // defined(__APPLE__) || defined(__linux__)
{
    reinterpret_cast<Thread*>(pThread)->Run();
    Exit();

#if defined(__APPLE__) || defined(__linux__)
    return NULL;
#elif defined(WIN32)
    return 0;
#endif // defined(__APPLE__) || defined(__linux__)
}

Cntl_NameSpace_End