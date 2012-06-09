// -*- mode: C++; coding: utf-8 -*-
#ifndef CNTL_MEMORYALLOCATOR_H_INCLUDED
#define CNTL_MEMORYALLOCATOR_H_INCLUDED

//
// An STL-compliant allocator template.
//
// Ref.:
//
// * Stack Allocator
//   http://src.chromium.org/viewvc/chrome/trunk/src/base/stack_container.h
//
// * Visual C++ Team Blog : The Mallocator
//   http://blogs.msdn.com/vcblog/archive/2008/08/28/the-mallocator.aspx
//
// * Pete Isensee "Custom STL Allocators"
//   http://www.tantalon.com/pete.htm
//

#include <cntl/Memory.h>

namespace cntl
{
    template <typename Type, std::size_t Align = 16>
    class STLAllocator
    {
    public:

        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;
        typedef Type*          pointer;
        typedef const Type*    const_pointer;
        typedef Type&          reference;
        typedef const Type&    const_reference;
        typedef Type           value_type;

        template <typename Other>
        struct rebind
        {
            typedef STLAllocator<Other, Align> other;
        };

        STLAllocator() throw()
            {}

        STLAllocator( const STLAllocator& other ) throw()
            {}

        template <typename OtherType, std::size_t OtherAlign>
        STLAllocator( const STLAllocator<OtherType, OtherAlign>& ) throw()
            {}

        ~STLAllocator() throw()
            {}

        pointer
        address( reference r ) const
            {
                return &r;
            }

        const_pointer
        address( const_reference cr ) const
            {
                return &cr;
            }

        pointer
        allocate( size_type count, const void* /* hint */ = 0 )
            {
                void* area = cntlMallocAlignedSTL( count * sizeof(Type), Align );
                return reinterpret_cast< pointer >( area );
            }

        void
        deallocate( pointer area, size_type /* count */ )
            {
                cntlFreeAlignedSTL( area );
            }

        void
        construct( pointer area, const_reference content )
            {
                ::new ( area ) Type( content );
            }

        void
        destroy( pointer area )
            {
                area->~Type();
            }

        size_type
        max_size() const
            {
                return std::size_t(-1) / sizeof(Type);
            }

        // non-standard interface
        std::size_t
        alignment()
            {
                return Align;
            }

    private:
        STLAllocator& operator =( const STLAllocator& other );
    };


    template <typename Type1, std::size_t Align1,
              typename Type2, std::size_t Align2>
    bool operator ==( const STLAllocator<Type1,Align1>& a1,
                      const STLAllocator<Type2,Align2>& a2 ) throw()
    {
        return Align1 == Align2;
    }

    template <typename Type1, std::size_t Align1,
              typename Type2, std::size_t Align2>
    bool operator !=( const STLAllocator<Type1,Align1>& a1,
                      const STLAllocator<Type2,Align2>& a2 ) throw()
    {
        return Align1 != Align2;
    }
}

#endif
