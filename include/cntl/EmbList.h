// -*- mode: C++; coding: utf-8-dos; -*-
#ifndef EMBLIST_H_INCLUDED
#define EMBLIST_H_INCLUDED

#include <cstddef>
#include <cassert>

namespace cntl
{
    struct EmbListLink {
        EmbListLink *next, *prev;
    };

    template <typename TNode, std::size_t NLinkOffset>
    struct EmbList
    {
        EmbListLink *head, *tail;

        inline void Initialize()
            { head = tail = NULL; }


        inline TNode* GetNode( EmbListLink* lnk )
            {
                return reinterpret_cast<TNode*>( reinterpret_cast<std::size_t>(lnk) - NLinkOffset );
            }

        inline TNode* GetNode( EmbListLink* lnk, EmbListLink TNode::* member_ptr )
            {
                std::size_t link_offset = reinterpret_cast<std::size_t>( &(reinterpret_cast<TNode*>(0)->*member_ptr) );
                return reinterpret_cast<TNode*>( reinterpret_cast<std::size_t>(lnk) - link_offset );
            }

        inline int GetNodeCount()
            {
                if ( head == NULL && tail == NULL )
                    return 0;

                int count = 1;
                for ( EmbListLink* pLink = head; pLink != tail; pLink = pLink->next )
                    ++count;

                return count;
            }

        inline TNode* GetHead()
            { return head ? GetNode( head ) : NULL; }

        inline TNode* GetTail()
            { return tail ? GetNode( tail ) : NULL; }

        inline TNode* GetNext( EmbListLink& lnk )
            { return lnk.next ? GetNode( lnk.next ) : NULL; }

        inline TNode* GetPrev( EmbListLink& lnk )
            { return lnk.prev ? GetNode( lnk.prev ) : NULL; }

        inline void PushFront( EmbListLink* lnk )
            {
                lnk->prev = NULL;
                lnk->next = head;
                (head ? head->prev : tail) = lnk;
                head = lnk;
            }

        inline void PushBack( EmbListLink* lnk )
            {
                lnk->prev = tail;
                lnk->next = NULL;
                (tail ? tail->next : head) = lnk;
                tail = lnk;
            }

        inline void InsertPrev( EmbListLink* lnk_pos, EmbListLink* lnk_new )
            {
                lnk_new->prev = lnk_pos->prev;
                lnk_new->next = lnk_pos;
                (lnk_pos->prev ? lnk_pos->prev->next : head) = lnk_new;
                lnk_pos->prev = lnk_new;
            }

        inline void InsertNext( EmbListLink* lnk_pos, EmbListLink* lnk_new )
            {
                lnk_new->prev = lnk_pos;
                lnk_new->next = lnk_pos->next;
                (lnk_pos->next ? lnk_pos->next->prev : tail) = lnk_new;
                lnk_pos->next = lnk_new;
            }

        inline void Remove( EmbListLink* lnk_del )
            {
                (lnk_del->prev ? lnk_del->prev->next : head) = lnk_del->next;
                (lnk_del->next ? lnk_del->next->prev : tail) = lnk_del->prev;
            }

        static inline bool IsAdjacent( EmbListLink* lnk_A, EmbListLink* lnk_B )
            {
                return (lnk_A->next == lnk_B) && (lnk_A == lnk_B->prev );
            }

        inline void MergeNotAdjacent( EmbListLink* lnk_merging, EmbListLink* lnk_takenover )
            {
                assert( lnk_merging < lnk_takenover );
                assert( !IsAdjacent(lnk_merging, lnk_takenover) );

                if ( lnk_takenover->prev == NULL )
                    head = lnk_merging;
                else
                    lnk_takenover->prev->next = lnk_merging;

                if ( lnk_takenover->next == NULL )
                    tail = lnk_merging;
                else
                    lnk_takenover->next->prev = lnk_merging;

                lnk_merging->prev = lnk_takenover->prev;
                lnk_merging->next = lnk_takenover->next;
            }

        inline void MergeAdjacent( EmbListLink* lnk_merging, EmbListLink* lnk_takenover )
            {
                assert( lnk_merging < lnk_takenover );
                assert( IsAdjacent(lnk_merging, lnk_takenover) );

                if ( lnk_takenover->next == NULL )
                    tail = lnk_merging;
                else
                    lnk_takenover->next->prev = lnk_merging;

                lnk_merging->next = lnk_takenover->next;
            }
    };

}

#endif
