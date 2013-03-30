// -*- mode: C++; coding: utf-8 -*-
#ifndef TCTLQUEUESHARED_H_INCLUDED
#define TCTLQUEUESHARED_H_INCLUDED

#include <cntl/TwoLockQueue.h>
#include <cntl/Thread.h>
#include <cntl/Timer.h>
#include <TestFramework.h>

static cntl::TwoLockQueue<int> tlq;

class TCTLQueueShared : public Test::Case
{
    const float tolerance;

public:

    static const int nProduct = 10000;

    class ProducerThread : public cntl::Thread
    {
    public:
        void Run()
            {
                while ( Suspended() )
                    WaitForResume();

                int iProduct = 0;

                while ( Runnable() )
                {
                    std::cout << "Producer produced:\t" << iProduct << std::endl;
                    tlq.Enqueue( iProduct++ );
                }

                std::cout << "Producer terminated." << std::endl;
            }
    };

    class ConsumerThread : public cntl::Thread
    {
    public:
        void Run()
            {
                while ( Suspended() )
                    WaitForResume();

                int iProduct = 0;
                bool dequeueSuceeded;

                while ( Runnable() )
                {
                    dequeueSuceeded = tlq.Dequeue( &iProduct );
                    if ( dequeueSuceeded )
                        std::cout << "Consumer received:\t" << iProduct << std::endl;
                    else
                        std::cout << "Shared queue is empty." << std::endl;
                }

                std::cout << "Consumer terminated." << std::endl;

                while ( (dequeueSuceeded = tlq.Dequeue( &iProduct ) ) == true )
                {
                    std::cout << iProduct << " was left in the queue." << std::endl;
                }
            }
    };

    ProducerThread m_threadProducer;
    ConsumerThread m_threadConsumer;

    TCTLQueueShared( const char* name )
        : Test::Case( name )
        , tolerance( 1.0e-6f )
        {}

    virtual void Run()
        {
            tlq.Initialize();

            m_threadProducer.Create(0, true);
            m_threadConsumer.Create(0, true);

            cntl::Timer timer;
            timer.Reset();
            m_threadConsumer.Resume();

            while ( timer.GetElapsedTime() < 1.0 );
            m_threadProducer.Resume();

            while ( timer.GetElapsedTime() < 10.0 );

            m_threadProducer.Terminate();
            m_threadConsumer.Terminate();

            m_threadProducer.Join();
            m_threadConsumer.Join();

            m_threadProducer.Destroy();
            m_threadConsumer.Destroy();

            tlq.Finalize();
        }
};

#endif
