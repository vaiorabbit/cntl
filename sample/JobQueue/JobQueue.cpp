#include <cassert>
#include <cntl/Memory.h>
#include "JobQueue.h"

JobQueue::Worker::Worker( JobQueue* pJobQueue )
    : m_pJobQueue( pJobQueue )
    , m_pJob( NULL )
{}

JobQueue::Worker::~Worker()
{}

void
JobQueue::Worker::Run()
{
    while ( Suspended() )
        WaitForResume();

    while ( Runnable() )
    {
        while ( Suspended() )
            WaitForResume();

        if ( m_pJob )
        {
            Job::Data* pJobData = m_pJob->GetData();
            Job::Processor pfnJobProc = m_pJob->GetProcessor();
            pfnJobProc( pJobData );

            pJobData->SetDoneFlagOn();
            m_pJob->~Job();
            cntlFreeSystem( m_pJob );
            m_pJob = NULL;
        }
        else
        {
            if ( m_pJobQueue->Running() )
            {
                m_pJob = m_pJobQueue->PopJob();
            }
            else
            {
                cntl::Thread::Sleep( 1 );
            }
        }
    }

    if ( m_pJob )
    {
        Job::Data* pJobData = m_pJob->GetData();
        pJobData->SetDoneFlagOff();
        m_pJob->~Job();
        cntlFreeSystem( m_pJob );
        m_pJob = NULL;
    }
}


JobQueue::JobQueue()
    : m_apWorkers()
    , m_qpJobs()
    , m_bRun( false )
{}

JobQueue::~JobQueue()
{}

void
JobQueue::Initialize( int nThreads )
{
    assert( nThreads > 0 );
    m_apWorkers.assign( nThreads, NULL );

    for ( int i = 0; i < nThreads; ++i )
    {
        Worker* pWorker = (Worker*)cntlMallocSystem( sizeof(Worker) );
        ::new ( pWorker ) Worker( this );
        m_apWorkers[i] = pWorker;
    }

    for ( WorkerArray::iterator it = m_apWorkers.begin(); it != m_apWorkers.end(); ++it )
    {
        (*it)->Create();
    }

    m_qpJobs.Initialize();
}

void
JobQueue::Finalize()
{
    for ( WorkerArray::iterator it = m_apWorkers.begin(); it != m_apWorkers.end(); ++it )
    {
        (*it)->Terminate();
    }

    for ( WorkerArray::iterator it = m_apWorkers.begin(); it != m_apWorkers.end(); ++it )
    {
        (*it)->Join();
    }

    for ( WorkerArray::iterator it = m_apWorkers.begin(); it != m_apWorkers.end(); ++it )
    {
        (*it)->Destroy();
        (*it)->~Worker();
        cntlFreeSystem( (*it) );
    }

    while ( !Empty() )
    {
        Job* pJob = PopJob();
        if ( pJob )
        {
            pJob->~Job();
            cntlFreeSystem( pJob );
        }
    }
    m_qpJobs.Finalize();
}

void
JobQueue::Start()
{
    for ( WorkerArray::iterator it = m_apWorkers.begin(); it != m_apWorkers.end(); ++it )
    {
        (*it)->Resume();
    }
    m_bRun = true;
}

void
JobQueue::Stop()
{
    m_bRun = false;
}

bool
JobQueue::WorkersIdle()
{
    for ( WorkerArray::iterator it = m_apWorkers.begin(); it != m_apWorkers.end(); ++it )
        if ( (*it)->HasJob() )
            return false;

    return true;
}

void
JobQueue::PushJob( Job::Data* pJobData, Job::Processor pJobProcessor )
{
    Job* pJob = (Job*)cntlMallocSystem( sizeof(Job) );
    ::new ( pJob ) Job( pJobData, pJobProcessor );
    m_qpJobs.Enqueue( pJob );
}

Job*
JobQueue::PopJob()
{
    Job* pJob = NULL;
    bool bPopSucceeded = m_qpJobs.Dequeue( &pJob );
    return bPopSucceeded ? pJob : NULL;
}
