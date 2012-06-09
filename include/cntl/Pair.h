// -*- mode: C++; coding: utf-8 -*-
#ifndef CNTL_PAIR_H_INCLUDED
#define CNTL_PAIR_H_INCLUDED

namespace cntl
{
    template <typename Type1, typename Type2>
    struct Pair
    {
        typedef Type1 first_type;
        typedef Type2 second_type;

        pair()
            : first( Type1() )
            , second( Type2() )
            {}

        pair( const Type1& val1, const Type2& val2 )
            : first( val1 )
            , second( val2 )
            {}

        template <typename OtherType1, typename OtherType2>
        pair( const pair<OtherType1, OtherType2>& other )
            : first( other.first )
            , second( other.second )
            {}

        Type1 first;
        Type2 second;
    };

    template <typename Type1, typename Type2>
    inline Pair<Type1, Type2>
    make_pair( const Type1& val1, const Type2& val2 )
    {
        return Pair<Type1, Type2>( val1, val2 );
    }

    // Binary Operators

    template <typename Type1, typename Type2>
    inline bool
    operator ==( const Pair<Type1, Type2>& lhs,
                 const Pair<Type1, Type2>& rhs )
    {
        return (lhs.first == rhs.first) && (lhs.second == rhs.second);
    }

    template <typename Type1, typename Type2>
    inline bool
    operator !=( const Pair<Type1, Type2>& lhs,
                 const Pair<Type1, Type2>& rhs )
    {
        return !(lhs == rhs);
    }

    template <typename Type1, typename Type2>
    inline bool
    operator  <( const Pair<Type1, Type2>& lhs,
                 const Pair<Type1, Type2>& rhs )
    {
        return
            ( lhs.first < rhs.first ) ||
            (!(lhs.first < rhs.first) && (lhs.second < rhs.second) );
    }

    template <typename Type1, typename Type2>
    inline bool
    operator  >( const Pair<Type1, Type2>& lhs,
                 const Pair<Type1, Type2>& rhs )
    {
        return rhs < lhs;
    }

    template <typename Type1, typename Type2>
    inline bool
    operator <=( const Pair<Type1, Type2>& lhs,
                 const Pair<Type1, Type2>& rhs )
    {
        return !( lhs > rhs );
    }

    template <typename Type1, typename Type2>
    inline bool
    operator >=( const Pair<Type1, Type2>& lhs,
                 const Pair<Type1, Type2>& rhs )
    {
        return !( lhs < rhs );
    }
}

/*
  Ref.:
    - pair
      - http://www.cplusplus.com/reference/std/utility/pair/
    - make_pair
      - http://www.cplusplus.com/reference/std/utility/make_pair/
 */

#endif
