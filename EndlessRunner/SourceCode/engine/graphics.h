#pragma once

typedef struct game_offscreen_buffer_t
{
    void *Memory;

    int Width;
    int Height;

    int Pitch;
    int BytesPerPixel;
} game_offscreen_buffer_t;

typedef struct bitmap_t
{
    void *Memory;

    int Width;
    int Height;

    int Pitch;
    int BytesPerPixel;
} bitmap_t;

struct CGraphicsManager
{
    static void ClearBuffer(game_offscreen_buffer_t *Buffer, float r, float g, float b);
    static unsigned int CGraphicsManager::BlendColor(unsigned int dst, unsigned int src);

    static void DrawBitmap(game_offscreen_buffer_t *Buffer, bitmap_t *Bitmap, float RealMinX, float RealMinY);
    static void DrawRectangle(game_offscreen_buffer_t *Buffer, float RealMinX, float RealMinY, float RealMaxX, float RealMaxY, float r, float g, float b);
};