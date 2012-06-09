// -*- mode: C++; coding: utf-8 -*-
#ifndef CNTL_LINKEDLIST_H_INCLUDED
#define CNTL_LINKEDLIST_H_INCLUDED

#include <algorithm>
#include <functional>
#include <iterator>
#include <new>
#include <cassert>
#include <cntl/STLAllocator.h>

//
// * Features
//
//   * Doubly-linked list implementation.
//
//   * Almost compatible with std::list.
//

namespace cntl
{
    template <typename Type, class Allocator = cntl::STLAllocator<Type, 16> >
    class LinkedList
    {
        struct LLNode
        {
            Type data;
            LLNode *prev, *next;

            LLNode()
                : data()
                , prev( NULL )
                , next( NULL )
                {}

            ~LLNode()
                {
                    uninstall();
                }

            void install( const Type& val )
                {
                    new ( &data ) Type( val ); // data[at] = val;
                }

            void uninstall()
                {
                    data.~Type();
                }
        };

        template <typename T, typename TPtr, typename TRef>
        class LLIteratorImpl
        {
        public:
            typedef std::bidirectional_iterator_tag iterator_category;

            typedef T              value_type;
            typedef TPtr           pointer;
            typedef TRef           reference;
            typedef std::ptrdiff_t difference_type;
            typedef std::ptrdiff_t distance_type;

            LLIteratorImpl( LLNode* node = NULL )
                : current_node(node)
                {}

            LLIteratorImpl( const LLIteratorImpl& other )
                : current_node(other.current_node)
                {}


            bool operator ==( const LLIteratorImpl& rhs ) const
                {
                    return current_node == rhs.current_node;
                }

            bool operator !=( const LLIteratorImpl& rhs ) const
                {
                    return !(*this == rhs);
                }

            reference operator *() const
                {
                    return current_node->data;
                }

            pointer operator ->() const
                {
                    return &(operator*());
                }

            LLIteratorImpl& operator ++()
                {
                    increment();
                    return *this;
                }

            LLIteratorImpl operator ++( int )
                {
                    LLIteratorImpl it = *this;
                    increment();
                    return it;
                }

            LLIteratorImpl& operator --()
                {
                    decrement();
                    return *this;
                }

            LLIteratorImpl operator --( int )
                {
                    LLIteratorImpl it = *this;
                    decrement();
                    return it;
                }

        private:

            void increment()
                {
                    current_node = current_node->next;
                }

            void decrement()
                {
                    current_node = current_node->prev;
                }

            void insert( const Type& val )
                {
                    current_node->insert( val );
                }

            friend class LinkedList;
            LLNode* current_node;
        };

    public:

        //
        // Types
        //

        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;
        typedef Type*          pointer;
        typedef const Type*    const_pointer;
        typedef Type&          reference;
        typedef const Type&    const_reference;
        typedef Type           value_type;

        typedef Allocator allocator_type;

        typedef LLIteratorImpl<Type, Type*, Type&> iterator;
        typedef LLIteratorImpl<Type, const Type*, const Type&> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        //
        // Construct / Copy / Destroy
        //

        LinkedList()
            : m_pHeadNode( ll_create_node() )
            {
                m_pHeadNode->prev = m_pHeadNode;
                m_pHeadNode->next = m_pHeadNode;
            }

        LinkedList( const LinkedList& other )
            : m_pHeadNode( ll_create_node() )
            {
                *this = other;
            }

        // LinkedList( int reserved_node_count )
        //     : m_pHeadNode( ll_create_node() )
        //     , m_pTailNode( m_pHeadNode )
        //     {}

        LinkedList& operator =( const LinkedList& other )
            {
                clear();
                assign( other.begin(), other.end() );

                return *this;
            }

        ~LinkedList()
            {
                clear();
                ll_destroy_node( m_pHeadNode );
            }

        //
        // Iterators
        //

        iterator begin()
            {
                return iterator( m_pHeadNode->next );
            }

        const_iterator begin() const
            {
                return const_iterator( m_pHeadNode->next );
            }

        iterator end()
            {
                return iterator( m_pHeadNode );
            }

        const_iterator end() const
            {
                return const_iterator( m_pHeadNode );
            }

        reverse_iterator rbegin()
            {
                return reverse_iterator( end() );
            }

        const_reverse_iterator rbegin() const
            {
                return const_reverse_iterator( end() );
            }

        reverse_iterator rend()
            {
                return reverse_iterator( begin() );
            }

        const_reverse_iterator rend() const
            {
                return const_reverse_iterator( begin() );
            }

        //
        // Capacity
        //

        bool empty() const
            {
                return begin() == end();
            }

        size_type size() const
            {
                size_type count = 0;
                for ( const_iterator it = begin(); it != end(); ++it )
                    ++count;

                return count;
            }

        size_type max_size() const
            {
                return get_allocator().max_size();
            }

        void resize( size_type new_size )
            {
                resize( new_size, Type() );
            }

        // [NOTE] The second argument of resize() is const_reference, not value_type.
        // This tweak (and cntl::STLAllocator) enables cntl::LinkedList to manage aligned objects.
        void resize( size_type new_size, const Type& val )
            {
                // +val+ is used only when +new_size+ is greater than size().
                iterator it_current = begin();
                iterator it_end = end();
                size_type count = 0;
                while ( new_size > count && it_current != it_end )
                {
                    ++it_current, ++count;
                }

                if ( count >= new_size )
                {
                    if ( it_current != it_end )
                        erase( it_current, it_end );
                }
                else // it_current == it_end
                {
                    int n = new_size - size();
                    if ( n > 0 )
                        insert( it_end, n, val );
                }
            }

        //
        // Element access
        //

        reference front()
            {
                return *begin();
            }

        const_reference front() const
            {
                return *begin();
            }

        reference back()
            {
                return *(--end());
            }

        const_reference back() const
            {
                return *(--end());
            }

        //
        // Modifiers
        //

        template <class InputIterator>
        void assign( InputIterator first, InputIterator last )
            {
                iterator it_current = begin();
                iterator it_end = end();
                while ( first != last && it_current != it_end )
                {
                    *it_current = *first;
                    ++it_current, ++first;
                }

                if ( first == last ) // source sequence is shorter than or equal to this list.
                {
                    if ( it_current != it_end )
                        erase( it_current, it_end );
                }
                else
                {
                    insert( it_end, first, last );
                }
            }

        void assign( size_type count, const Type& val )
            {
                iterator it_current = begin();
                iterator it_end = end();
                while ( count > 0 && it_current != it_end )
                {
                    *it_current = val;
                    ++it_current, --count;
                }

                if ( count == 0 ) // source sequence is shorter than or equal to this list.
                {
                    if ( it_current != it_end )
                        erase( it_current, it_end );
                }
                else
                {
                    insert( it_end, count, val );
                }
            }

        void push_front( const Type& val )
            {
                insert( begin(), val );
            }

        void pop_front()
            {
                erase( begin() );
            }

        void push_back( const Type& val )
            {
                insert( end(), val );
            }

        void pop_back()
            {
                erase( --end() );
            }


        // Inserts +val+ before the iterator +pos+.
        iterator insert( iterator pos, const Type& val )
            {
                LLNode* new_node = ll_create_node();

                new_node->next = pos.current_node;
                new_node->prev = pos.current_node->prev;

                pos.current_node->prev->next = new_node;
                pos.current_node->prev = new_node;

                new_node->install( val );

                return iterator(new_node);
            }

        void insert( iterator pos, size_type count, const Type& val )
            {
                while ( count-- )
                {
                    pos = insert( pos, val );
                    ++pos;
                }
            }

        template <class InputIterator>
        void insert( iterator pos, InputIterator first, InputIterator last )
            {
                for ( ; first != last; ++first )
                {
                    pos = insert( pos, *first );
                    ++pos;
                }
            }

        // pos : [ begin(), end() )
        iterator erase( iterator pos )
            {
                if ( empty() )
                    return pos;

                LLNode* unused =  pos.current_node;
                LLNode *p = unused->prev, *n = unused->next;

                p->next = n;
                n->prev = p;

                ll_destroy_node( unused );

                return iterator(n);
            }

        // Erases [first, last)
        iterator erase( iterator first, iterator last )
            {
                while ( first != last )
                    erase( first++ );

                return last;
            }

        void swap( LinkedList& other )
            {
                std::swap( m_pHeadNode, other.m_pHeadNode );
            }

        void clear()
            {
                assert( m_pHeadNode );

                LLNode *unused = m_pHeadNode->next;
                while ( unused && unused != m_pHeadNode )
                {
                    LLNode* next = unused->next;
                    ll_destroy_node( unused );
                    unused = next;
                }

                m_pHeadNode->uninstall();
                m_pHeadNode->prev = m_pHeadNode;
                m_pHeadNode->next = m_pHeadNode;
            }

        //
        // Operations
        //

        void splice( iterator pos, LinkedList& val )
            {
                if ( &val != this && !val.empty() )
                {
                    LLNode* other_s = val.begin().current_node;
                    LLNode* other_t = (--val.end()).current_node;
                    assert( other_t->next->next == other_s );

                    other_s->prev->next = other_t->next;
                    other_t->next->prev = other_s->prev;

                    pos.current_node->prev->next = other_s;
                    other_s->prev = pos.current_node->prev;

                    pos.current_node->prev = other_t;
                    other_t->next = pos.current_node;

                    assert( val.empty() );
                }
            }

        void splice( iterator pos, LinkedList& val, iterator i )
            {
                if ( &val != this && !val.empty() )
                {
                    LLNode* other = i.current_node;
                    other->prev->next = other->next;
                    other->next->prev = other->prev;

                    pos.current_node->prev->next = other;
                    other->prev = pos.current_node->prev;

                    pos.current_node->prev = other;
                    other->next = pos.current_node;
                }
            }

        void splice( iterator pos, LinkedList& val, iterator first, iterator last )
            {
                if ( &val != this && !val.empty() )
                {
                    LLNode* other_s = first.current_node;
                    LLNode* other_t = (--last).current_node;

                    other_s->prev->next = other_t->next;
                    other_t->next->prev = other_s->prev;

                    pos.current_node->prev->next = other_s;
                    other_s->prev = pos.current_node->prev;

                    pos.current_node->prev = other_t;
                    other_t->next = pos.current_node;
                }
            }

        void remove( const Type& val )
            {
                iterator it_current = begin();
                while ( it_current != end() )
                {
                    if ( *it_current == val )
                    {
                        it_current = erase( it_current );
                    }
                    else
                    {
                        ++it_current;
                    }
                }
            }

        template <class Predicate>
        void remove_if( Predicate pred )
            {
                iterator it_current = begin();
                while ( it_current != end() )
                {
                    if ( pred( *it_current ) )
                    {
                        it_current = erase( it_current );
                    }
                    else
                    {
                        ++it_current;
                    }
                }
            }

        // Removes duplicated elements.
        // [NOTE] The list should be sorted before using this function
        //        because removal occurs only when two *consecutive*
        //        elements have the same content.
        void unique()
            {
                iterator it_current = begin();
                for ( iterator it_next = ++begin(); it_next != end(); ++it_next )
                {
                    if ( *it_current == *it_next )
                    {
                        it_current = erase( it_next );
                        --it_current;
                    }
                    else
                    {
                        ++it_current;
                    }
                    it_next = it_current;
                }
            }

        template <typename BinaryPredicate>
        void unique( BinaryPredicate );
/*
            {
                iterator it_current = begin();
                for ( iterator it_next = ++begin(); it_next != end(); ++it_next )
                {
                    if ( binary_pred(*it_current, *it_next) )
                    {
                        it_current = erase( it_next );
                        --it_current;
                    }
                    else
                    {
                        ++it_current;
                    }
                    it_next = it_current;
                }
            }
*/

        void merge( LinkedList& val )
            {
                if ( &val != this && !val.empty() )
                {
                    iterator it_current = begin();
                    iterator it_src_current = val.begin();
                    while ( it_current != end() && it_src_current != val.end() )
                    {
                        if ( *it_src_current < *it_current )
                        {
                            LLNode* other = it_src_current.current_node;
                            it_src_current.current_node = it_src_current.current_node->next;

                            other->prev->next = other->next;
                            other->next->prev = other->prev;

                            it_current.current_node->prev->next = other;
                            other->prev = it_current.current_node->prev;

                            it_current.current_node->prev = other;
                            other->next = it_current.current_node;

                            it_current.current_node = other;

                            // it_current = insert( it_current, *it_src_current );
                            // it_src_current = val.erase( it_src_current );
                        }
                        else
                        {
                            ++it_current;
                        }
                    }

                    if ( it_src_current != val.end() ) // +val+ is longer than this list
                    {
                        splice( end(), val, it_src_current, val.end() );

                        // insert( end(), it_src_current, val.end() );
                        // val.erase( it_src_current, val.end() );
                    }
                }
            }

        template <class Compare>
        void merge( LinkedList& val, Compare comp )
            {
                if ( &val != this && !val.empty() )
                {
                    iterator it_current = begin();
                    iterator it_src_current = val.begin();
                    while ( it_current != end() && it_src_current != val.end() )
                    {
                        if ( comp(*it_src_current, *it_current) )
                        {
                            LLNode* other = it_src_current.current_node;
                            it_src_current.current_node = it_src_current.current_node->next;

                            other->prev->next = other->next;
                            other->next->prev = other->prev;

                            it_current.current_node->prev->next = other;
                            other->prev = it_current.current_node->prev;

                            it_current.current_node->prev = other;
                            other->next = it_current.current_node;

                            it_current.current_node = other;

                            // it_current = insert( it_current, *it_src_current );
                            // it_src_current = val.erase( it_src_current );
                        }
                        else
                        {
                            ++it_current;
                        }
                    }

                    if ( it_src_current != val.end() ) // +val+ is longer than this list
                    {
                        splice( end(), val, it_src_current, val.end() );

                        // insert( end(), it_src_current, val.end() );
                        // val.erase( it_src_current, val.end() );
                    }
                }
            }

        void sort()
            {
                if ( size() < 2 )
                    return;

                LLNode* n_begin = begin().current_node;
                LLNode* n_end = end().current_node;
                LLNode* n_current = n_begin->next;

                while ( n_current != n_end )
                {
                    if ( !( n_current->prev->data < n_current->data ) )
                    {
                        LLNode* n_backup = n_current->next;

                        LLNode* n_pos = n_current->prev;
                        while ( n_pos != m_pHeadNode && !( n_pos->data < n_current->data ) )
                            n_pos = n_pos->prev;

                        n_current->next->prev = n_current->prev;
                        n_current->prev->next = n_current->next;

                        n_current->next = n_pos->next;
                        n_current->prev = n_pos;

                        n_pos->next->prev = n_current;
                        n_pos->next = n_current;

                        n_current = n_backup;
                    }
                    else
                        n_current = n_current->next;
                }
            }

        template <class Compare>
        void sort( Compare comp )
            {
                if ( size() < 2 )
                    return;

                LLNode* n_begin = begin().current_node;
                LLNode* n_end = end().current_node;
                LLNode* n_current = n_begin->next;

                while ( n_current != n_end )
                {
                    if ( !comp( n_current->prev->data, n_current->data ) )
                    {
                        LLNode* n_backup = n_current->next;

                        LLNode* n_pos = n_current->prev;
                        while ( n_pos != m_pHeadNode && !comp( n_pos->data, n_current->data ) )
                            n_pos = n_pos->prev;

                        n_current->next->prev = n_current->prev;
                        n_current->prev->next = n_current->next;

                        n_current->next = n_pos->next;
                        n_current->prev = n_pos;

                        n_pos->next->prev = n_current;
                        n_pos->next = n_current;

                        n_current = n_backup;
                    }
                    else
                        n_current = n_current->next;
                }
            }

        void reverse()
            {
                iterator from_head = begin();
                iterator from_tail = end();
                do
                {
                    --from_tail;
                    if ( from_head == from_tail )
                        break;
                    std::swap( *from_head, *from_tail );
                    ++from_head;
                } while ( from_head != from_tail );
            }

        //
        // Allocator
        //

        allocator_type get_allocator() const
            {
                return allocator_type();
            }

    private:

        typedef typename Allocator::template rebind<LLNode>::other LLNodeAllocator;

        LLNodeAllocator ll_get_node_allocator() const
            {
                return LLNodeAllocator();
            }

        LLNode* ll_create_node()
            {
                LLNodeAllocator ma = ll_get_node_allocator();
                LLNode* node = (LLNode*)ma.allocate( 1 );
                assert( node );
                new ( node ) LLNode;
                return node;
            }

        void ll_destroy_node( LLNode* node )
            {
                node->~LLNode();
                LLNodeAllocator ma = ll_get_node_allocator();
                ma.deallocate( node, 1 );
            }

        // dummy node for circular list. head->next==begin(), head==end().
        LLNode* m_pHeadNode;
    };

    //
    // Binary operators
    //

    template <typename Type, class Allocator>
    inline bool
    operator ==( const LinkedList<Type, Allocator>& lhs,
                 const LinkedList<Type, Allocator>& rhs )
    {
        if ( lhs.size() != rhs.size() )
            return false;

        typename LinkedList<Type,Allocator>::const_iterator l_current = lhs.begin(), l_end = lhs.end();
        typename LinkedList<Type,Allocator>::const_iterator r_current = rhs.begin(), r_end = rhs.end();

        while ( l_current != l_end && r_current != r_end )
        {
            if ( *l_current != *r_current )
                return false;

            ++l_current, ++r_current;
        }

        return l_current == l_end && r_current == r_end ? true : false;
    }

    template <typename Type, class Allocator>
    inline bool
    operator !=( const LinkedList<Type, Allocator>& lhs,
                 const LinkedList<Type, Allocator>& rhs )
    {
        return !(lhs == rhs);
    }

    template <typename Type, class Allocator>
    inline bool
    operator  <( const LinkedList<Type, Allocator>& lhs,
                 const LinkedList<Type, Allocator>& rhs )
    {
        return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
    }

    template <typename Type, class Allocator>
    inline bool
    operator <=( const LinkedList<Type, Allocator>& lhs,
                 const LinkedList<Type, Allocator>& rhs )
    {
        return !(lhs > rhs);
    }

    template <typename Type, class Allocator>
    inline bool
    operator  >( const LinkedList<Type, Allocator>& lhs,
                 const LinkedList<Type, Allocator>& rhs )
    {
        return rhs < lhs;
    }

    template <typename Type, class Allocator>
    inline bool
    operator >=( const LinkedList<Type, Allocator>& lhs,
                 const LinkedList<Type, Allocator>& rhs )
    {
        return !(lhs < rhs);
    }

    template <typename Type, class Allocator>
    void swap( LinkedList<Type, Allocator>& lhs, LinkedList<Type, Allocator>& rhs )
    {
        lhs.swap( rhs );
    }

    template <typename Type, class Allocator>
    template <typename BinaryPredicate>
    void LinkedList<Type,Allocator>::unique( BinaryPredicate binary_pred )
    {
        iterator it_current = begin();
        for ( iterator it_next = ++begin(); it_next != end(); ++it_next )
        {
            if ( binary_pred(*it_current, *it_next) )
            {
                it_current = erase( it_next );
                --it_current;
            }
            else
            {
                ++it_current;
            }
            it_next = it_current;
        }
    }

}

#endif
