// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef CNTL_THREAD_H_INCLUDED
#define CNTL_THREAD_H_INCLUDED

#if defined(__APPLE__) || defined(__linux__)
# include <pthread.h>
#elif defined(WIN32)
# include <windows.h>
#endif

namespace cntl
{
    class Thread
    {
    public:
        Thread();
        virtual ~Thread();
        bool Create( unsigned int nStackSize = 0, bool bCreateSuspended = false );
        void Terminate();
        void Destroy();
        void Join();

        void Suspend();
        void Resume();
        bool Suspended();

    protected:
        virtual void Run() =0;
        bool Runnable();
        void WaitForResume();

    private:
#if defined(__APPLE__) || defined(__linux__)
        static void* BootThread( void* pThread );
#elif defined(WIN32)
        // Ref.: LPTHREAD_START_ROUTINE Function Pointer (http://msdn.microsoft.com/en-us/library/aa964928.aspx)
        //       typedef DWORD (__stdcall *LPTHREAD_START_ROUTINE) ( [in] LPVOID lpThreadParameter );
        static DWORD __stdcall BootThread( LPVOID pThread );
#endif
        static void Exit();

#if defined(__APPLE__) || defined(__linux__)
        pthread_t m_hThread;
        pthread_mutex_t m_mutexRunnable;
        bool m_bRunnable;

        pthread_mutex_t m_mutexSuspend;
        pthread_cond_t m_condResume;
        bool m_bSuspended;
#elif defined(WIN32)
        HANDLE m_hThread;
        CRITICAL_SECTION m_mutexRunnable;
        bool m_bRunnable;

        CRITICAL_SECTION m_mutexSuspend;
        bool m_bSuspended;
#endif
    };
}
#endif // CNTL_THREAD_H_INCLUDED
