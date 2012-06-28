// -*- mode: C++; coding: utf-8 -*-
#ifndef CNTL_STRING_H_INCLUDED
#define CNTL_STRING_H_INCLUDED

//
// * Yet another C string wrapper.
//

#include <ostream>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <cntl/Memory.h>

namespace cntl
{
    class String
    {
    public:

        static const std::size_t npos = -1;

        // Constructor

        String()
            : m_cstr( cs_emptystr()/*cs_duplicate( "" )*/ )
            {}

        String( const String& str )
            : m_cstr( cs_duplicate( str.c_str() ) )
            {}

        String( const char* s )
            : m_cstr( cs_duplicate( s ) )
            {}

        ~String()
            {
                if ( !cs_isnotallocated() )
                    cntlFreeSystem( m_cstr );
            }

        String& operator =( const String& other )
            {
                if ( this != &other )
                {
                    char* new_cstr = cs_duplicate( other.c_str() );
                    if ( !cs_isnotallocated() )
                        cntlFreeSystem( m_cstr );
                    if ( new_cstr != NULL  )
                        m_cstr = new_cstr;
                }

                return *this;
            }

        String& operator =( const char* s )
            {
                if ( m_cstr != s )
                {
                    char* new_cstr = cs_duplicate( s );
                    if ( !cs_isnotallocated() )
                        cntlFreeSystem( m_cstr );
                    if ( new_cstr != NULL  )
                        m_cstr = new_cstr;
                }

                return *this;
            }

        // Capacity

        std::size_t size() const
            {
                return cs_length( m_cstr );
            }

        void clear()
            {
                if ( cs_isnotallocated() )
                {
                    m_cstr = cs_emptystr();
                }
                else
                {
                    cntlFreeSystem( m_cstr );
                    m_cstr = cs_duplicate( "" );
                }
            }

        bool empty() const
            {
                return cs_length( m_cstr ) == 0;
            }

        // Element access

        const char& operator []( std::size_t pos ) const
            {
                return m_cstr[pos];
            }

        char& operator []( std::size_t pos )
            {
                return m_cstr[pos];
            }

        // Modifiers

        String& operator +=( const String& str )
            {
                std::size_t old_length = cs_length( m_cstr );
                std::size_t src_length = cs_length( str.m_cstr );

                if ( cs_isnotallocated() )
                    m_cstr = static_cast<char*>( cntlMallocSystem( old_length+src_length+1 ) );
                else
                    m_cstr = static_cast<char*>( cntlReallocSystem( m_cstr, old_length+src_length+1 ) );
                assert( m_cstr != NULL );
                std::memmove( m_cstr+old_length, str.m_cstr, src_length+1 );

                return *this;
            }

        String& operator +=( const char* s )
            {
                std::size_t old_length = cs_length( m_cstr );
                std::size_t src_length = cs_length( s );

                if ( cs_isnotallocated() )
                    m_cstr = static_cast<char*>( cntlMallocSystem( old_length+src_length+1 ) );
                else
                    m_cstr = static_cast<char*>( cntlReallocSystem( m_cstr, old_length+src_length+1 ) );
                assert( m_cstr != NULL );
                std::memmove( m_cstr+old_length, s, src_length+1 );

                return *this;
            }

        String& erase( std::size_t pos = 0, std::size_t n = npos )
            {
                if ( !cs_isnotallocated() )
                {
                    std::size_t old_length = cs_length( m_cstr );
                    assert( pos <= old_length );
                    if ( pos > old_length )
                        return *this;
                    std::size_t m = old_length - pos;
                    std::size_t l = n < m ? n : m;
                    if ( l > 0 )
                    {
                        char* s = m_cstr + pos;
                        char* t = s + l;
                        std::size_t move_count = m - l + 1;
                        std::memmove( s, t, move_count );
                        m_cstr = static_cast<char*>( cntlReallocSystem( m_cstr, old_length-l +1) );
                        assert( m_cstr != NULL );
                    }
                }
                
                return *this;
            }

        void swap( String& str )
            {
                char* tmp = m_cstr;
                m_cstr = str.m_cstr;
                str.m_cstr = tmp;
            }

        // String operations
        const char* c_str() const
            {
                return m_cstr;
            }

        String substr( size_t pos = 0, size_t n = npos ) const
            {
                if ( cs_isnotallocated() )
                    return String(m_cstr);
                
                std::size_t original_length = cs_length( m_cstr );
                assert( pos <= original_length );

                std::size_t sub_length = n == npos ? original_length - pos : n;
                std::size_t sub_length_with_nullchar = sub_length + 1;

                char* dst = static_cast<char*>( cntlMallocSystem(sub_length_with_nullchar) );
                assert( dst != NULL );
                std::memcpy( dst, m_cstr+pos, sub_length );
                *(dst+sub_length) = '\0';

                String result( dst );
                cntlFreeSystem( dst );

                return result;
            }

        std::size_t find( const String& str, std::size_t pos = 0 ) const
            {
                char* p = std::strstr( m_cstr+pos, str.m_cstr );
                return p ? p - m_cstr : npos;
            }

        std::size_t find( const char* s, std::size_t pos = 0 ) const
            {
                char* p = std::strstr( m_cstr+pos, s );
                return p ? p - m_cstr : npos;
            }

        std::size_t find( char c, std::size_t pos = 0 ) const
            {
                char* p = std::strchr( m_cstr+pos, c );
                return p ? p - m_cstr : npos;
            }


        std::size_t find_first_of( const String& str, std::size_t pos = 0 ) const
            {
                return find_first_of( str.c_str(), pos );
            }

        std::size_t find_first_of( const char* s, std::size_t pos = 0 ) const
            {
                if ( pos >= cs_length(m_cstr) )
                    return npos;

                char* p = std::strpbrk( m_cstr+pos, s );
                return p ? p - m_cstr : npos;
            }

        std::size_t find_first_of( char c, std::size_t pos = 0 ) const
            {
                const char s[] = { c, '\0' };
                return find_first_of( s, pos );
            }


        std::size_t find_first_not_of( const String& str, std::size_t pos = 0 ) const
            {
                return find_first_not_of( str.c_str(), pos );
            }

        std::size_t find_first_not_of( const char* s, std::size_t pos = 0 ) const
            {
                std::size_t length = cs_length(m_cstr);

                if ( pos >= length )
                    return npos;

                std::size_t sub_length = length - pos;

                std::size_t l = std::strspn( m_cstr+pos, s );

                return  l == sub_length ? npos : l + pos;
            }

        std::size_t find_first_not_of( char c, std::size_t pos = 0 ) const
            {
                const char s[] = { c, '\0' };
                return find_first_not_of( s, pos );
            }

    private:

        char* m_cstr;
        
        static char* cs_emptystr()
        {
            static char* s_pEmptyStr = "";
            return s_pEmptyStr;
        }
        
        bool cs_isnotallocated() const
        {
            return m_cstr == cs_emptystr();
        }

        static std::size_t cs_length( const char* s )
            {
                std::size_t c = 0;
                while ( *s++ != '\0' )
                    ++c;

                return c;
            }

        char* cs_duplicate( const char* src )
            {
                std::size_t length_with_nullchar = cs_length( src ) + 1;
                char* dst = static_cast<char*>( cntlMallocSystem(length_with_nullchar) );
                assert( dst != NULL );
                std::memcpy( dst, src, length_with_nullchar );

                return dst;
            }

    }; // class String

    // Binary operators

    inline bool
    operator ==( const String& lhs, const String& rhs )
    {
        return std::strcmp( lhs.c_str(), rhs.c_str() ) == 0;
    }

    inline bool
    operator !=( const String& lhs, const String& rhs )
    {
        return !(lhs == rhs);
    }

    inline bool
    operator  <( const String& lhs, const String& rhs )
    {
        return std::strcmp( lhs.c_str(), rhs.c_str() ) < 0;
    }

    inline bool
    operator  >( const String& lhs, const String& rhs )
    {
        return rhs < lhs;
    }

    inline bool
    operator <=( const String& lhs, const String& rhs )
    {
        return !( lhs > rhs );
    }

    inline bool
    operator >=( const String& lhs, const String& rhs )
    {
        return !( rhs > lhs );
    }


    std::ostream&
    operator <<( std::ostream& os, const String& s )
    {
        os << s.c_str();

        return os;
    }
}

#endif
