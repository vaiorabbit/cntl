// -*- mode: C++; coding: utf-8 -*-
#ifndef VECTOR3_H_INCLUDED
#define VECTOR3_H_INCLUDED

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
};

#endif
