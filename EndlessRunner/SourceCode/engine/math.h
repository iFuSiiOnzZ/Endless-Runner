#pragma once

/*
    Project includes
*/
#include "macros.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/

typedef union Vec2f
{
    struct { float x, y; };
    float v[2];

    Vec2f() : x(0.0f), y(0.0f) {}
    Vec2f(float xx, float yy) : x(xx), y(yy) {}
    Vec2f(const Vec2f &Vector) : x(Vector.x), y(Vector.y) {}
} Vec2f;

inline Vec2f operator * (const Vec2f &Vec1, float v)
{
    return Vec2f(Vec1.x * v, Vec1.y * v);
}

inline Vec2f operator / (const Vec2f &Vec1, float v)
{
    ASSERT(v > 0.00001f, "Division by zero");
    float iv = 1.0f / v;

    return Vec2f(Vec1.x * iv, Vec1.y * iv);
}

inline Vec2f operator + (const Vec2f &Vec1, const Vec2f &Vec2)
{
    return Vec2f(Vec1.x + Vec2.x, Vec1.y + Vec2.y);
}

inline Vec2f operator - (const Vec2f &Vec1, const Vec2f &Vec2)
{
    return Vec2f(Vec1.x + Vec2.x, Vec1.y + Vec2.y);
}

inline Vec2f operator * (const Vec2f &Vec1, const Vec2f &Vec2)
{
    return Vec2f(Vec1.x * Vec2.x, Vec1.y * Vec2.y);
}

inline Vec2f operator / (const Vec2f &Vec1, const Vec2f &Vec2)
{
    return Vec2f(Vec1.x / Vec2.x, Vec1.y / Vec2.y);
}

///////////////////////////////////////////////////////////////////////////////

typedef struct Rect2d
{
    Vec2f UpperLeft;
    Vec2f RightBottom;

    inline Vec2f GetSize()
    {
        return Vec2f(RightBottom.x - UpperLeft.x, RightBottom.y - UpperLeft.y);
    }
} Rect2d;


