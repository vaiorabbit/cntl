// -*- mode: C++; coding: utf-8 -*-
#ifndef VECTOR3_H_INCLUDED
#define VECTOR3_H_INCLUDED

#include <cmath>

#if defined(_MSC_VER)
# define VECTOR3_ALIGNMENT_DECL __declspec(align(16))
#elif  defined(__GNUC__)
# define VECTOR3_ALIGNMENT_DECL __attribute__((aligned(16)))
#else
# define VECTOR3_ALIGNMENT_DECL
#endif

struct VECTOR3_ALIGNMENT_DECL Vector3
{
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f)
    {}

    Vector3( float x_, float y_, float z_ ) : x(x_), y(y_), z(z_)
    {}

    /*explicit*/ Vector3( const Vector3& other ) : x(other.x), y(other.y), z(other.z)
    {}

    ~Vector3()
    {}

    Vector3& operator =( const Vector3& other )
    {
        if ( this != &other )
        {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    float LengthSqared() const
    {
        return x*x + y*y + z*z;
    }

    float Length() const
    {
        return std::sqrt(LengthSqared());
    }

    friend bool operator <( const Vector3& lhs, const Vector3& rhs );
    friend bool operator ==( const Vector3& lhs, const Vector3& rhs );
};

bool operator <( const Vector3& lhs, const Vector3& rhs )
{
    return lhs.LengthSqared() < rhs.LengthSqared();
}

bool operator <=( const Vector3& lhs, const Vector3& rhs )
{
    return !(rhs < lhs);
}

bool operator >( const Vector3& lhs, const Vector3& rhs )
{
    return rhs < lhs;
}

bool operator >=( const Vector3& lhs, const Vector3& rhs )
{
    return !(lhs < rhs);
}

bool operator ==( const Vector3& lhs, const Vector3& rhs )
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator !=( const Vector3& lhs, const Vector3& rhs )
{
    return !(lhs == rhs);
}



#endif
