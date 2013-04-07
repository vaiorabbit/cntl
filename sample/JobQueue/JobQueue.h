#ifndef JOBQUEUE_H_INCLUDED
#define JOBQUEUE_H_INCLUDED

#include <cntl/DynamicArray.h>
#include <cntl/Thread.h>
#include <cntl/TwoLockQueue.h>

class Job;
class JobQueue;


class Job
{
public:
    class Data
    {
    public:
        Data()
            : m_bDoneProcessing(false)
            {}

        virtual ~Data()
            {}

        bool Done() const
            { return m_bDoneProcessing; }

        void SetDoneFlag( bool bDone )
            { m_bDoneProcessing = bDone; }

        void SetDoneFlagOn()
            { SetDoneFlag(true); }

        void SetDoneFlagOff()
            { SetDoneFlag(false); }

    private:
        bool m_bDoneProcessing;
    };

    typedef void (*Processor)(Data* pData);

    Job( Data* pData, Processor pProcessor )
        : m_pData(pData)
        , m_pfnProcessor(pProcessor)
        {}

    ~Job()
        {};

    Data* GetData()
        { return m_pData; }

    Processor GetProcessor()
        { return m_pfnProcessor; }

private:
    Data* m_pData;
    Processor m_pfnProcessor;
};


class JobQueue
{
public:

    class Worker : public cntl::Thread
    {
    public:
        Worker( JobQueue* pJobQueue );
        ~Worker();
        void Run();

    private:
        JobQueue* m_pJobQueue;
        Job* m_pJob;
    };

    JobQueue();
    ~JobQueue();
    void Initialize( int nThreads );
    void Finalize();

    void Start();
    void Stop();
    bool Running()
        { return m_bRun; }

    void PushJob( Job::Data* pJobData, Job::Processor pJobProcessor );
    Job* PopJob();
    bool Empty()
        { return m_qpJobs.Empty(); }

private:

    typedef cntl::DynamicArray<Worker*> WorkerArray;
    WorkerArray m_apWorkers;
    cntl::TwoLockQueue<Job*> m_qpJobs;
    bool m_bRun;
};

#endif // JOBQUEUE_H_INCLUDED
