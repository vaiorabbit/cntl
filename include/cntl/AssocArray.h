// -*- mode: C++; coding: utf-8 -*-
#ifndef CNTL_ASSOCARRAY_H_INCLUDED
#define CNTL_ASSOCARRAY_H_INCLUDED

//
// * Features
//
//   * Ordered assosiative map implemented with cntl::DynamicArray.
//
//   * Almost compatible with std::map.
//
//   * Non-standard API reserve() offers to allocate sufficient memory
//     area in advance.
//

#include <algorithm>
#include <functional>
#include <cntl/STLAllocator.h>
#include <cntl/DynamicArray.h>

namespace cntl
{
    template <class Key, class Type, class Compare = std::less<Key>,
              class Allocator = cntl::STLAllocator<std::pair<Key, Type>, 16> >
    class AssocArray
    {
    public:

        //
        // Types
        //

        typedef Key                  key_type;
        typedef Type                 mapped_type;
        typedef std::pair<Key, Type> value_type;
        typedef Compare              key_compare;
        typedef Allocator            allocator_type;

        typedef value_type&       reference;
        typedef const value_type& const_reference;

        typedef typename Allocator::pointer       pointer;
        typedef typename Allocator::const_pointer const_pointer;

        class value_compare : public std::binary_function<value_type, value_type, bool>
        {
            friend class AssocArray;

        protected:
            Compare comp;
            value_compare( Compare c ) : comp(c) {}

        public:
            bool operator ()( const value_type& x, const value_type& y ) const
                {
                    return comp( x.first, y.first );
                }

            // [NOTE] Non-standard
            bool operator ()( const key_type& x, const value_type& y ) const
                {
                    return operator()( x, y.first );
                }

            // [NOTE] Non-standard
            bool operator ()( const value_type& x, const key_type& y ) const
                {
                    return operator()( x.first, y );
                }

            // [NOTE] Non-standard
            bool operator ()( const key_type& x, const key_type& y ) const
                {
                    return comp( x, y );
                }
        }; // class value_compare

    private:

        typedef DynamicArray<value_type, allocator_type> Impl;
        Impl m_impl;

        value_compare m_value_comp;

    public:

        //
        // Types
        //

        typedef typename Impl::iterator         iterator;
        typedef typename Impl::const_iterator   const_iterator;
        typedef typename Impl::size_type        size_type;
        typedef typename Impl::difference_type  difference_type;
        typedef typename Impl::reverse_iterator reverse_iterator;
        typedef typename Impl::reverse_iterator const_reverse_iterator;

        //
        // Construct / Copy / Destroy
        //

        explicit AssocArray( const Compare& comp = Compare(),
                             const Allocator& alloc = Allocator() )
            : m_impl( alloc )
            , m_value_comp( comp )
            {}

        template <class InputIterator>
        AssocArray( InputIterator first, InputIterator last,
                    const Compare& comp = Compare(), const Allocator& alloc = Allocator() )
            : m_impl( first, last, alloc )
            , m_value_comp( comp )
            {
                std::sort( m_impl.begin(), m_impl.end(), m_value_comp );
            }

        AssocArray( const AssocArray<Key,Type,Compare,Allocator>& x )
            : m_impl( x.m_impl )
            , m_value_comp( x.m_value_comp )
            {}

        ~AssocArray()
            {
                m_impl.clear();
            }

        AssocArray<Key,Type,Compare,Allocator>&
        operator =( const AssocArray<Key,Type,Compare,Allocator>& x )
            {
                if ( this != &x )
                {
                    m_impl = x.m_impl;
                    m_value_comp = x.m_value_comp;
                }

                return *this;
            }

        //
        // Iterators
        //

        iterator begin()
            {
                return m_impl.begin();
            }

        const_iterator begin() const
            {
                return m_impl.begin();
            }

        iterator end()
            {
                return m_impl.end();
            }

        const_iterator end() const
            {
                return m_impl.end();
            }

        reverse_iterator rbegin()
            {
                return m_impl.rbegin();
            }

        const_reverse_iterator rbegin() const
            {
                return m_impl.rbegin();
            }

        reverse_iterator rend()
            {
                return m_impl.rend();
            }

        const_reverse_iterator rend() const
            {
                return m_impl.rend();
            }

        //
        // Capacity
        //

        bool empty() const
            {
                return m_impl.empty();
            }

        size_type size() const
            {
                return m_impl.size();
            }

        size_type max_size() const
            {
                return m_impl.max_size();
            }

        //
        // [NOTE] Non-standard API
        //

        void reserve( size_type count )
            {
                m_impl.reserve( count );
            }

        //
        // Element access
        //

        mapped_type& operator []( const key_type& x )
            {
                // lower_bound returns +it+ such that x <= *it.
                iterator it = lower_bound( x );

                // - unable to find x, or
                // - x < *it (means "*it isn't equal to x"),
                // then insert new element with default constructor.
                if ( it == end() || m_value_comp.comp( x, (*it).first ) )
                {
                    it = insert( it, value_type( x, mapped_type() ) );
                }

                return (*it).second;
            }

        //
        // Modifiers
        //

        std::pair<iterator, bool> insert( const value_type& x )
            {
                bool actually_inserted = false;
                iterator it = lower_bound( x.first );        // lower_bound returns +it+ such that x <= *it.

                if ( it == end() ||                          // If all resinding elements are less than x, or
                     m_value_comp.comp(x.first, (*it).first) // x < *it  (means x != *it) then add x as a new element.
                    )
                {
                    it = m_impl.insert( it, x );
                    actually_inserted = true;
                }

                return std::make_pair( it, actually_inserted );
            }

        iterator insert( iterator position, const value_type& x )
            {
                // +position+ is just a hint for insertion point search.

                //
                // Ref.: GCC libstdc++-v3 HOWTO: Chapter 23: Containers
                // "Hinting" during insertion
                //
                // > If the hint parameter ('p' above) is equivalent to:
                // >
                // > begin(), then the item being inserted should have a key less than
                // > all the other keys in the container. The item will be inserted at
                // > the beginning of the container, becoming the new entry at begin().
                if ( position == begin() )
                {
                    if ( position != (iterator)0 && m_value_comp(x, *position) )
                        return m_impl.insert( position, x );
                    else
                        return insert( x ).first;
                }
                // > end(), then the item being inserted should have a key greater than
                // > all the other keys in the container. The item will be inserted at
                // > the end of the container, becoming the new entry at end().
                else if ( position == end() )
                {
                    if ( position != (iterator)0 && m_value_comp(*(position-1), x) )
                        return m_impl.insert( position, x );
                    else
                        return insert( x ).first;
                }
                // > neither begin() nor end(), then: Let h be the entry in the container
                // > pointed to by hint, that is, h = *hint. Then the item being inserted
                // > should have a key less than that of h, and greater than that of the item
                // > preceding h. The new item will be inserted between h and h's predecessor.
                else
                {
                    // But, in general, it's inefficient to insert an element into cntl::DynamicArray
                    // at arbitrary position (except for push_back). Even if we got the hint about
                    // appropriate range for insertion, there's no mean to make use of that.
                    return insert( x ).first;
                }
            }

        template <class InputIterator>
        void insert( InputIterator first, InputIterator last )
            {
                for ( ; first != last; ++first )
                {
                    insert( *first );
                }
            }

        void erase( iterator position )
            {
                m_impl.erase( position );
            }

        size_type erase( const key_type& x )
            {
                iterator it = find( x );
                if ( it == end() )
                {
                    return 0;
                }
                else
                {
                    erase( it );
                    return 1;
                }
            }

        void erase( iterator first, iterator last )
            {
                m_impl.erase( first, last );
            }

        void swap( AssocArray<Key,Type,Compare,Allocator>& x )
            {
                m_impl.swap( x.m_impl );
            }

        void clear()
            {
                m_impl.clear();
            }

        //
        // Observers
        //

        key_compare key_comp() const
            {
                return m_value_comp.comp;
            }

        value_compare value_comp() const
            {
                return m_value_comp;
            }

        //
        // Operations
        //

        iterator find( const key_type& x )
            {
                // lower_bound returns +it+ such that x <= *it.
                iterator it = lower_bound( x );

                // - unable to find x, or
                // - x < *it (means "*it isn't equal to x"),
                // then report failure by returning end().
                return it == end() || (it != end() && m_value_comp.comp( x, (*it).first )) ? end() : it;
            }

        const_iterator find( const key_type& x ) const
            {
                // lower_bound returns +it+ such that x <= *it.
                const_iterator it = lower_bound( x );

                // - unable to find x, or
                // - x < *it (means "*it isn't equal to x"),
                // then report failure by returning end().
                return it == end() || (it != end() && m_value_comp.comp( x, (*it).first )) ? end() : it;
            }

        size_type count( const key_type& x ) const
            {
                return find( x ) != end() ? 1 : 0;
            }

        iterator lower_bound( const key_type& x )
            {
                return std::lower_bound( begin(), end(), x, m_value_comp );
            }

        const_iterator lower_bound( const key_type& x ) const
            {
                return std::lower_bound( begin(), end(), x, m_value_comp );
            }

        iterator upper_bound( const key_type& x )
            {
                return std::upper_bound( begin(), end(), x, m_value_comp );
            }

        const_iterator upper_bound( const key_type& x ) const
            {
                return std::upper_bound( begin(), end(), x, m_value_comp );
            }

        std::pair<iterator, iterator>
        equal_range( const key_type& x )
            {
                return std::equal_range( begin(), end(), x, m_value_comp );
            }

        std::pair<const_iterator, const_iterator>
        equal_range( const key_type& x ) const
            {
                return std::equal_range( begin(), end(), x, m_value_comp );
            }

        template <class K, class T, class C, class A>
        friend bool operator ==( const AssocArray<K,T,C,A>& lhs, const AssocArray<K,T,C,A>& rhs );

        template <class K, class T, class C, class A>
        friend bool operator <( const AssocArray<K,T,C,A>& lhs, const AssocArray<K,T,C,A>& rhs );

        //
        // Allocator
        //

        allocator_type get_allocator() const
            {
                return m_impl.get_allocator();
            }

    }; // class AssocArray

    //
    // Binary operators
    //

    template <class Key, class Type, class Compare, class Allocator>
    bool operator ==( const AssocArray<Key,Type,Compare,Allocator>& lhs, const AssocArray<Key,Type,Compare,Allocator>& rhs )
    {
        return lhs.m_impl == rhs.m_impl;
    }

    template <class Key, class Type, class Compare, class Allocator>
    bool operator !=( const AssocArray<Key,Type,Compare,Allocator>& lhs, const AssocArray<Key,Type,Compare,Allocator>& rhs )
    {
        return !(lhs == rhs);
    }

    template <class Key, class Type, class Compare, class Allocator>
    bool operator <( const AssocArray<Key,Type,Compare,Allocator>& lhs, const AssocArray<Key,Type,Compare,Allocator>& rhs )
    {
        return lhs.m_impl < rhs.m_impl;
    }

    template <class Key, class Type, class Compare, class Allocator>
    bool operator >( const AssocArray<Key,Type,Compare,Allocator>& lhs, const AssocArray<Key,Type,Compare,Allocator>& rhs )
    {
        return rhs < lhs;
    }

    template <class Key, class Type, class Compare, class Allocator>
    bool operator >=( const AssocArray<Key,Type,Compare,Allocator>& lhs, const AssocArray<Key,Type,Compare,Allocator>& rhs )
    {
        return !(lhs < rhs);
    }

    template <class Key, class Type, class Compare, class Allocator>
    bool operator <=( const AssocArray<Key,Type,Compare,Allocator>& lhs, const AssocArray<Key,Type,Compare,Allocator>& rhs )
    {
        return !(rhs < lhs);
    }

    template <class Key, class Type, class Compare, class Allocator>
    void swap( AssocArray<Key,Type,Compare,Allocator>& lhs, AssocArray<Key,Type,Compare,Allocator>& rhs )
    {
        lhs.swap( rhs );
    }

} // namespace cntl


