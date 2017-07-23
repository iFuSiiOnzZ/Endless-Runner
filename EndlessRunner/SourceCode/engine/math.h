#pragma once

typedef union Vec2f
{
    struct { float x, y; };
    float v[2];
} Vec2f;

///////////////////////////////////////////////////////////////////////////////

typedef struct Rect2d
{
    Vec2f position; // position: x, y
    Vec2f size;     // size: x -> width, y -> height
} Rect2d;
