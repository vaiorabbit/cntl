#include <iostream>
#include <cmath>
#include <cntl/Memory.h>
#include <cntl/Timer.h>
#include "JobQueue.h"

#define DATA0_INSTANCE_MAX 10000
#define DATA0_CONTENT_MAX 10000

class JobData0 : public Job::Data
{
public:
    float data[DATA0_CONTENT_MAX];
};

JobData0 cJobData0[DATA0_INSTANCE_MAX];

void JobProc0( Job::Data* pData )
{
    JobData0* pData0 = (JobData0*)pData;
    for ( int i = 0; i < DATA0_CONTENT_MAX; ++i )
    {
        pData0->data[i] = std::exp((float)i);
    }
}

int main( int argc, char** argv )
{
    cntl::MemoryAllocatorProperty property;
    property.m_uAreaSizeForSystemHeap = 8 * 1024 * 1024;
    property.m_uAreaSizeForSTLHeap = 8 * 1024 * 1024;
    cntl::MemoryAllocatorInitialize( property );

    {
        JobQueue cJQ;
        cJQ.Initialize( 4 );

        for ( int i = 0; i < DATA0_INSTANCE_MAX; ++i )
        {
            cJQ.PushJob( &cJobData0[i], JobProc0 );
        }

        cntl::Timer timer;
        timer.Reset();

        cJQ.Start();
        while ( !cJQ.Empty() )
        {
            cntl::Thread::Sleep( 1 );
        };
        std::cout << "GetElapsedTime=" << timer.GetElapsedTime() << std::endl;

        cJQ.Stop();
        cJQ.Finalize();
    }

    cntl::MemoryAllocatorFinalize();

    return 0;
}
