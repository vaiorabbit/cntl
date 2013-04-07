// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef CNTL_TWOLOCKQUEUE_H_INCLUDED
#define CNTL_TWOLOCKQUEUE_H_INCLUDED

//
// Ref.: Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue Algorithms
//       http://www.cs.rochester.edu/research/synchronization/pseudocode/queues.html
//       http://www.cs.rochester.edu/u/scott/papers/1996_PODC_queues.pdf
//

#include <cassert>
#include <cntl/Mutex.h>
#include <cntl/STLAllocator.h>

namespace cntl
{
    template <typename Type, class Allocator = cntl::STLAllocator<Type, 16> >
    class TwoLockQueue
    {
    public:

        struct Node
        {
            Type value;
            Node* next;
        };

        typedef typename Allocator::template rebind<Node>::other NodeAllocator;
        typedef typename Allocator::template rebind<Mutex>::other MutexAllocator;

        TwoLockQueue()
            : m_pHead(NULL)
            , m_pTail(NULL)
            , m_pMutexHead(NULL)
            , m_pMutexTail(NULL)
            {}

        virtual ~TwoLockQueue()
            {}

        void Initialize()
            {
                NodeAllocator nma = get_node_allocator();
                Node* pNode = nma.allocate( 1 );
                assert( pNode );
                ::new ( pNode ) Node;

                pNode->next = NULL;
                m_pHead = m_pTail = pNode;

                MutexAllocator mma = get_mutex_allocator();
                m_pMutexHead = mma.allocate( 1 );
                assert( m_pMutexHead );
                ::new ( m_pMutexHead ) Mutex;

                m_pMutexTail = mma.allocate( 1 );
                assert( m_pMutexTail );
                ::new ( m_pMutexTail ) Mutex;
            }

        void Finalize()
            {
                while ( !Empty() )
                    Dequeue( NULL );

                m_pHead->~Node();
                NodeAllocator nma = get_node_allocator();
                nma.deallocate( m_pHead, 1 );

                MutexAllocator mma = get_mutex_allocator();
                m_pMutexTail->~Mutex();
                mma.deallocate( m_pMutexTail, 1 );
                m_pMutexHead->~Mutex();
                mma.deallocate( m_pMutexHead, 1 );
            }

        void Enqueue( const Type& value )
            {
                NodeAllocator nma = get_node_allocator();
                Node* pNode = nma.allocate( 1 );
                assert( pNode );
                ::new ( pNode ) Node;

                pNode->value = value;
                pNode->next = NULL;
                m_pMutexTail->Lock();
                {
                    m_pTail->next = pNode;
                    m_pTail = pNode;
                }
                m_pMutexTail->Unlock();
            }

        bool Dequeue( Type* pValueOut )
            {
                m_pMutexHead->Lock();
                Node* pNode = m_pHead;
                Node* pNewHead = pNode->next;
                if ( pNewHead == NULL )
                {
                    m_pMutexHead->Unlock();
                    return false;
                }

                if ( pValueOut )
                    *pValueOut = pNewHead->value;
                m_pHead = pNewHead;
                m_pMutexHead->Unlock();

                pNode->~Node();
                NodeAllocator nma = get_node_allocator();
                nma.deallocate( pNode, 1 );

                return true;
            }

        bool Empty()
            {
                return m_pHead->next == NULL;
            }

    private:

        NodeAllocator get_node_allocator() const
            {
                return NodeAllocator();
            }

        MutexAllocator get_mutex_allocator() const
            {
                return MutexAllocator();
            }

        Node* m_pHead;
        Node* m_pTail;
        Mutex* m_pMutexHead;
        Mutex* m_pMutexTail;
    };
}

#endif
