    // -*- mode: C++; coding: utf-8 -*-
#ifndef CNTL_DYNAMICARRAY_H_INCLUDED
#define CNTL_DYNAMICARRAY_H_INCLUDED

//
// * Features
//
//   * Reinvented std::vector with some modifications:
//
//     * The second argument of resize() is const_reference, not value_type
//       -> enables to hold aligned objects.
//       Ref.: error C2719: alignment problem - GameDev.Net Discussion Forums
//             http://www.gamedev.net/community/forums/topic.asp?topic_id=252754
//
//     * push_back() (and insert() series) does't double the capacity,
//       but expands only to the sufficient capacity enough to keep added elements.
//       -> might lead to slow execution speed. Use reserve() if possible.
//
// * Prerequisites
//
//   * +Type+ must provide value semantics. i.e. :
//     * Default constructor : Type()
//     * Copy constructor : Type( const Type& other )
//     * Assignment operator : Type::operator =( const Type& other )
//
//   * +Type+ must provide these binary operators : ==, !=, <, <=, >, >=
//       * operator ==( const Type& lhs, const Type& rhs )
//       * operator !=( const Type& lhs, const Type& rhs )
//       * operator  <( const Type& lhs, const Type& rhs )
//       * operator <=( const Type& lhs, const Type& rhs )
//       * operator  >( const Type& lhs, const Type& rhs )
//       * operator >=( const Type& lhs, const Type& rhs )
//
// * Notes
//
//   * get_allocator().allocate() might throw std::bad_alloc() exception.
//
//   * clear() and erase() don't decrease capacity() (same as std::vector implementation).
//     Use "shrink-to-fit idiom" if necessary:
//
//     [Example]
//       DynamicArray<int> v;
//       v.reserve( 5963 );                // v.size() == 0,    v.capcity() == 5963
//       v.resize( 4649, 0xdeadbeef );     // v.size() == 4649, v.capcity() == 5963
//       DynamicArray<int>( v ).swap( v ); // v.size() == 4649, v.capcity() == 4649
//

#include <iterator>
#include <cassert>
#include <cntl/STLAllocator.h>

namespace cntl
{
    template <typename Type, class Allocator = cntl::STLAllocator<Type, 16> >
    class DynamicArray
    {
    public:

        //
        // Standard types
        //

        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;
        typedef Type*          pointer;
        typedef const Type*    const_pointer;
        typedef Type&          reference;
        typedef const Type&    const_reference;
        typedef Type           value_type;

        typedef Allocator         allocator_type;
        typedef value_type*       iterator;
        typedef const value_type* const_iterator;

        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        //
        // Constructor / Destructor
        //

        explicit
        DynamicArray( const Allocator& = Allocator() )
            : m_pStorageBegin( 0 )
            , m_pStorageEnd( 0 )
            , m_pContentEnd( 0 )
            {}

        explicit
        DynamicArray( size_type count, const Type& val = Type(), const Allocator& = Allocator() )
            {
                pointer area = get_allocator().allocate( count );
                assert( area );

                m_pStorageBegin = area;
                m_pStorageEnd   = area + count;
                m_pContentEnd   = da_construct_copy_ctor( area, count, val );
            }

        template <class InputIterator>
        DynamicArray( InputIterator first, InputIterator last, const Allocator& = Allocator() )
            {
                size_type count = size_type( last - first );

                pointer area = get_allocator().allocate( count );
                assert( area );

                m_pStorageBegin = area;
                m_pStorageEnd   = area + count;
                m_pContentEnd   = da_copy_from_iter( area, first, count );
            }

        DynamicArray( const DynamicArray& other )
            {
                size_type count = other.size();

                pointer area = get_allocator().allocate( count );
                assert( area );

                m_pStorageBegin = area;
                m_pStorageEnd   = area + count;
                m_pContentEnd   = da_copy_from_iter( area, other.begin(), count );
            }

        DynamicArray& operator =( const DynamicArray& other )
            {
                if ( this != &other )
                {
                    size_type other_size = other.size();
                    if ( other_size == 0 )
                    {
                        clear();
                    }
                    else if ( other_size <= size() ) // 0 < other_size <= size()
                    {
                        // source size <= dest size : erases excess elements in dest array.
                        size_type this_size = size();
                        m_pContentEnd = da_copy_from_iter( begin(), other.begin(), other_size ); // reset members; note this invalidates existing iterators.
                        da_destruct( m_pContentEnd, this_size - other_size );
                    }
                    else if ( other_size <= capacity() ) // size() < other_size <= capacity()
                    {
                        // source size <= dest capacity : contents of dest array are overwritten.
                        m_pContentEnd = da_copy_from_iter( begin(), other.begin(), other_size ); // reset members; note this invalidates existing iterators.
                    }
                    else // other_size > capacity()
                    {
                        // There's no sufficient capacity in dest array :
                        // contents of source array are copied to newly allocated area.
                        allocator_type ma = get_allocator();
                        pointer area = ma.allocate( other_size );
                        assert( area );

                        da_destruct( begin(), size() );
                        if ( da_memory_allocated() )
                            ma.deallocate( begin(), capacity() );

                        // reset members; note this invalidates existing iterators.
                        m_pStorageBegin = area;
                        m_pStorageEnd   = area + other_size;
                        m_pContentEnd   = da_copy_from_iter( area, other.begin(), other_size );
                    }
                }

                return *this;
            }

        ~DynamicArray()
            {
                da_destruct( begin(), size() );
                if ( da_memory_allocated() )
                    get_allocator().deallocate( begin(), capacity() );

                m_pStorageBegin = 0;
                m_pStorageEnd   = 0;
                m_pContentEnd   = 0;
            }

        //
        // Deletion methods
        //

        // Erases all elements
        void clear()
            {
                erase( begin(), end() );
            }

        // Erases a single element at +pos+, where +pos+ is in [begin(), end()).
        iterator erase( iterator pos )
            {
                return erase( pos, pos+1 );
            }

        // Erases elements in range [first, last).
        iterator erase( iterator first, iterator last )
            {
                if ( da_memory_allocated() && size() > 0 &&
                     begin() <= first && first < end() &&
                     begin() < last && last <= end() && first < last )
                {
                    pointer pNewContentEnd = da_copy_from_iter( first, last, size_type(m_pContentEnd - last) );
                    da_destruct( pNewContentEnd, size_type(m_pContentEnd - pNewContentEnd) );

                    // reset members; note this invalidates existing iterators.
                    m_pContentEnd = pNewContentEnd;
                }
                return first;
            }

        //
        // Size / Capacity manipulation
        //

        void reserve( size_type count )
            {
                if ( count <= max_size() && capacity() < count )
                {
                    allocator_type ma = get_allocator();
                    pointer area = ma.allocate( count );
                    assert( area );

                    pointer pNewContentEnd = area;

                    if ( size() > 0 )
                    {
                        pNewContentEnd = da_copy_from_iter( area, begin(), size() );

                        da_destruct( begin(), size() );
                    }

                    if ( da_memory_allocated() )
                        ma.deallocate( begin(), capacity() );

                    m_pStorageBegin = area;
                    m_pStorageEnd   = area + count;
                    m_pContentEnd   = pNewContentEnd;
                }
            }

        void resize( size_type new_size )
            {
                resize( new_size, Type() );
            }

        // [NOTE] The second argument of resize() is const_reference, not value_type.
        // This tweak (and cntl::STLAllocator) enables cntl::DynamicArray to manage aligned objects.
        void resize( size_type new_size, const_reference val )
            {
                size_type this_size = size();


                if ( this_size < new_size )
                    insert( end(), new_size - this_size, val );
                else if ( this_size == new_size )
                    return;
                else // this_size > new_size
                    erase( begin() + new_size, end() );
            }

        //
        // Iterators
        //

        const_iterator begin() const
            {
                return m_pStorageBegin;
            }

        iterator begin()
            {
                return m_pStorageBegin;
            }

        iterator end()
            {
                return m_pContentEnd;
            }

        const_iterator end() const
            {
                return m_pContentEnd;
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
        // Accessors
        //

        reference front()
            {
                return *m_pStorageBegin;
            }

        const_reference front() const
            {
                return *m_pStorageBegin;
            }

        reference back()
            {
                return *(m_pContentEnd - 1);
            }

        const_reference back() const
            {
                return *(m_pContentEnd - 1);
            }

        reference operator []( size_type index )
            {
                return *(m_pStorageBegin + index);
            }

        const_reference operator []( size_type index ) const
            {
                return *(m_pStorageBegin + index);
            }

        reference at( size_type index )
            {
                assert( index < size() );
                return (*this)[index];
            }

        const_reference at( size_type index ) const
            {
                assert( index < size() );
                return (*this)[index];
            }


        //
        // Insert / Push / Pop
        //

        // Inserts a single element at +pos+, and returns an iterator
        // that points the newly inserted element.
        iterator insert( iterator pos, const value_type& val )
            {
                return da_insert( pos, 1, val );
            }

        // Inserts elements at +pos+.
        void insert( iterator pos, size_type count, const value_type& val )
            {
                da_insert( pos, count, val );
            }

        // Inserts a range of elements [first, last).
        template <class InputIterator>
        void insert( iterator pos, InputIterator first, InputIterator last )
            {
                size_type growth = size_type( last - first );

                if ( growth == 0 )
                    return;

                size_type new_size = growth + size();

                if ( new_size <= capacity() )
                {
                    // There's sufficient capacity available
                    // -> move latter part to make room for new elements.

                    // move latter part of this array to make rooms at
                    // +pos+; note that element at the end moves first
                    // so not to overwrite existing elements.
                    size_type latter_count = size_type( end() - pos );
                    iterator src = end();
                    iterator dst = src + growth;
                    for ( size_type i = 0; i < latter_count; ++i )
                    {
                        ::new ( --dst ) value_type( *(--src) );
                    }

                    // copy
                    da_copy_from_iter( pos, first, growth );

                    // reset members; note this invalidates existing iterators.
                    m_pContentEnd += growth;
                }
                else
                {
                    // There's no sufficient capacity
                    // -> move/copy all elements into newly allocated area.
                    //
                    // Note that by adding this line;
                    //   new_size = 2 * size();
                    // allocation strategy becomes equivalent to that of std::vector.
                    allocator_type ma = get_allocator();
                    pointer area = ma.allocate( new_size );
                    assert( area );

                    size_type former_count = size_type( pos - begin() );
                    size_type latter_count = size_type( end() - pos );
                    assert( former_count + latter_count == size() );

                    pointer current = area;

                    // move former part of preexisting elements.
                    da_copy_from_iter( current, begin(), former_count );
                    current += former_count;

                    // copy specified element.
                    da_copy_from_iter( current, first, growth );
                    current += growth;

                    // move latter part of preexisting elements.
                    da_copy_from_iter( current, pos, latter_count );
                    current += latter_count;

                    // deallocate previous area.
                    erase( begin(), end() );
                    if ( da_memory_allocated() )
                        ma.deallocate( begin(), capacity() );

                    // reset members; note this invalidates existing iterators.
                    m_pStorageBegin = area;
                    m_pStorageEnd   = area + new_size;
                    m_pContentEnd   = current;
                }
            }

        // Inserts one element at the end.
        void push_back( const Type& val )
            {
                da_insert( end(), 1, val );
            }

        // Erases the last element.
        void pop_back()
            {
                if ( empty() )
                    return;

                erase( end() - 1 );
            }

        // Replaces all the elements with +count+ times of copy of +val+.
        void assign( size_type count, const Type& val )
            {
                clear();
                da_insert( begin(), count, val );
            }

        // Replaces all the elements with elements in the range [first, last).
        template <class InputIterator>
        void assign( InputIterator first, InputIterator last )
            {
                clear();
                insert( begin(), first, last );
            }


        //
        // Size query
        //

        size_type capacity() const
            {
                return size_type( m_pStorageEnd - m_pStorageBegin );
            }

        bool empty() const
            {
                return m_pStorageBegin == m_pContentEnd;
            }

        size_type max_size() const
            {
                return get_allocator().max_size();
            }

        size_type size() const
            {
                return size_type( m_pContentEnd - m_pStorageBegin );
            }


        //
        // Miscellaneous
        //

        allocator_type get_allocator() const
            {
                return allocator_type();
            }

        void swap( DynamicArray& other )
            {
                if ( this != &other )
                {
                    pointer tmp;
                    tmp                   = m_pStorageBegin;
                    m_pStorageBegin       = other.m_pStorageBegin;
                    other.m_pStorageBegin = tmp;

                    tmp                 = m_pStorageEnd;
                    m_pStorageEnd       = other.m_pStorageEnd;
                    other.m_pStorageEnd = tmp;

                    tmp                 = m_pContentEnd;
                    m_pContentEnd       = other.m_pContentEnd;
                    other.m_pContentEnd = tmp;
                }
            }

    private:

        bool da_memory_allocated()
            {
                return (m_pStorageBegin != 0) && (m_pStorageEnd != 0);
            }

        // [NOTE] does nothing if count == 0.
        pointer da_construct_copy_ctor( pointer area, size_type count, const value_type& val )
            {
                while ( count-- > 0 )
                    ::new ( area++ ) value_type( val );

                return area;
            }

        // [NOTE] does nothing if count == 0.
        void da_destruct( pointer area, size_type count )
            {
                while ( count-- > 0 )
                    ( area++ )->~Type(); // Invoke destructor
            }

        // [NOTE] does nothing if count == 0.
        iterator da_copy_from_iter( iterator dst, const_iterator src, size_type count )
            {
                while ( count-- > 0 )
                    ::new ( dst++ ) value_type( *(src++) ); // Invoke copy constructor

                return dst;
            }

        // [NOTE] does nothing if count == 0.
        iterator da_copy_from_val( iterator dst, const value_type& val, size_type count )
            {
                while ( count-- > 0 )
                    ::new ( dst++ ) value_type( val ); // Invoke copy constructor

                return dst;
            }

        // provides implementation for
        // - insert( pos, val ),
        // - insert( pos, count, val ),
        // - push_back( val ), and
        // - assign( pos, count, val ).
        // return : an iterator that points the newly inserted element.
        iterator da_insert( iterator pos, size_type count, const value_type& val )
            {
                if ( count == 0 )
                    return pos;

                size_type new_size = count + size();

                if ( new_size <= capacity() )
                {
                    // There's sufficient capacity available
                    // -> move latter part to make room for new elements.

                    // move latter part of this array to make rooms at
                    // +pos+; note that element at the end moves first
                    // so not to overwrite existing elements.
                    size_type latter_count = size_type( end() - pos );
                    iterator src = end();
                    iterator dst = src + count;
                    for ( size_type i = 0; i < latter_count; ++i )
                    {
                        ::new ( --dst ) value_type( *(--src) );
                    }

                    // copy
                    da_copy_from_val( pos, val, count );

                    // reset members; note this invalidates existing iterators.
                    m_pContentEnd += count;

                    return pos;
                }
                else
                {
                    // There's no sufficient capacity
                    // -> move/copy all elements into newly allocated area.
                    //
                    // Note that by adding this line;
                    //   new_size = 2 * size();
                    // allocation strategy becomes equivalent to that of std::vector.
                    allocator_type ma = get_allocator();
                    pointer area = ma.allocate( new_size );
                    assert( area );

                    size_type former_count = size_type( pos - begin() );
                    size_type latter_count = size_type( end() - pos );
                    assert( former_count + latter_count == size() );

                    pointer current = area;

                    // move former part of preexisting elements.
                    da_copy_from_iter( current, begin(), former_count );
                    current += former_count;

                    // copy specified element.
                    da_copy_from_val( current, val, count );
                    current += count;

                    // move latter part of preexisting elements.
                    da_copy_from_iter( current, pos, latter_count );
                    current += latter_count;

                    // deallocate previous area.
                    erase( begin(), end() );

                    if ( da_memory_allocated() )
                        ma.deallocate( begin(), capacity() );

                    // reset members; note this invalidates existing iterators.
                    m_pStorageBegin = area;
                    m_pStorageEnd   = area + new_size;
                    m_pContentEnd   = current;

                    return begin() + former_count;
                }
            }

        pointer m_pStorageBegin; // refers to the head of reserved area.
        pointer m_pStorageEnd;   // refers to the end of reserved area.
        pointer m_pContentEnd;   // refers to past-the-end element.

    }; // class DynamicArray

    //
    // Binary operators
    //

    template <class Type, class Allocator>
    inline bool
    operator ==( const DynamicArray<Type, Allocator>& lhs,
                 const DynamicArray<Type, Allocator>& rhs )
    {
        typename DynamicArray<Type, Allocator>::size_type s = lhs.size();
        if ( s != rhs.size() )
            return false;

        for ( typename DynamicArray<Type, Allocator>::size_type i = 0; i < s; ++i )
        {
            if ( lhs[i] != rhs[i] )
                return false;
        }

        return true;
    }

    template <typename Type, typename Allocator>
    inline bool
    operator !=( const DynamicArray<Type, Allocator>& lhs,
                 const DynamicArray<Type, Allocator>& rhs )
    {
        return !(lhs == rhs);
    }

    template <typename Type, typename Allocator>
    inline bool
    operator  <( const DynamicArray<Type, Allocator>& lhs,
                 const DynamicArray<Type, Allocator>& rhs )
    {
        // Lexicographical less-than comparison.
        // Ref.: http://www.cplusplus.com/reference/algorithm/lexicographical_compare/

        typename DynamicArray<Type,Allocator>::const_iterator l_current = lhs.begin(), l_end = lhs.end();
        typename DynamicArray<Type,Allocator>::const_iterator r_current = rhs.begin(), r_end = rhs.end();

        // sequentially compare both elements that have the same position.
        while ( l_current != l_end )
        {
            // if l_i>r_i or rhs is shorter than lhs, then return false.
            if ( r_current == r_end || *l_current > *r_current  )
                return false;
            // if l_i < r_i, then return true.
            else if ( *l_current < *r_current )
                return true;
            // else if l_i==r_i, then compare next elements.
            ++l_current;
            ++r_current;
        }

        // Both container have the same elements in the range [l_first, l_end],
        // Then the result is true if rhs is longer than lhs.
        return r_current != r_end;
    }

    template <typename Type, typename Allocator>
    inline bool
    operator  >( const DynamicArray<Type, Allocator>& lhs,
                 const DynamicArray<Type, Allocator>& rhs )
    {
        return rhs < lhs;
    }

    template <typename Type, typename Allocator>
    inline bool
    operator <=( const DynamicArray<Type, Allocator>& lhs,
                 const DynamicArray<Type, Allocator>& rhs )
    {
        return !( lhs > rhs );
    }

    template <typename Type, typename Allocator>
    inline bool
    operator >=( const DynamicArray<Type, Allocator>& lhs,
                 const DynamicArray<Type, Allocator>& rhs )
    {
        return !( lhs < rhs );
    }

} // namespace cntl

#endif
