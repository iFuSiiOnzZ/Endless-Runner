#include "graphics.h"

void CGraphicsManager::ClearBuffer(game_offscreen_buffer_t * Buffer, float r, float g, float b)
{
    unsigned int *Dest = ((unsigned int *)Buffer->Memory);
    unsigned int ClearColor = ((unsigned int)(r * 255.0f) << 16) | ((unsigned int)(g * 255.0f) << 8) | ((unsigned int)(b * 255.0f) << 0);

    for (int i = 0; i < Buffer->Height * Buffer->Width; ++i)
    {
        *Dest++ = ClearColor;
    }
}

unsigned int CGraphicsManager::BlendColor(unsigned int dst, unsigned int src)
{
    unsigned char r1 = (dst & 0x00FF0000) >> 16;
    unsigned char g1 = (dst & 0x0000FF00) >> 8;
    unsigned char b1 = (dst & 0x000000FF) >> 0;

    unsigned char r2 = (src & 0x00FF0000) >> 16;
    unsigned char g2 = (src & 0x0000FF00) >> 8;
    unsigned char b2 = (src & 0x000000FF) >> 0;

    float a = (src >> 24) / 255.0f;
    unsigned char r = (unsigned char)((1 - a) * r1 + a * r2);
    unsigned char g = (unsigned char)((1 - a) * g1 + a * g2);
    unsigned char b = (unsigned char)((1 - a) * b1 + a * b2);

    return r << 16 | g << 8 | b << 0;
}

void CGraphicsManager::DrawRectangle(game_offscreen_buffer_t *Buffer, float RealMinX, float RealMinY, float RealMaxX, float RealMaxY, float r, float g, float b)
{
    int MinX = (int)(RealMinX + 0.5f);
    int MinY = (int)(RealMinY + 0.5f);
    int MaxX = (int)(RealMaxX + 0.5f);
    int MaxY = (int)(RealMaxY + 0.5f);

    if (MinX < 0) MinX = 0;
    if (MinY < 0) MinY = 0;

    if (MaxX > Buffer->Width) MaxX = Buffer->Width;
    if (MaxY > Buffer->Height) MaxY = Buffer->Height;


    unsigned int Color = ((unsigned int)(r * 255.0f)) << 16 | ((unsigned int)(g * 255.0f)) << 8 | ((unsigned int)(b * 255.0f)) << 0;
    unsigned char *Row = ((unsigned char *)Buffer->Memory + MinX * Buffer->BytesPerPixel + MinY * Buffer->Pitch);

    for (int Y = MinY; Y < MaxY; ++Y)
    {
        unsigned int *Pixel = (unsigned int *)Row;
        for (int X = MinX; X < MaxX; ++X)
        {
            *Pixel++ = Color;
        }

        Row += Buffer->Pitch;
    }
}

void CGraphicsManager::DrawBitmap(game_offscreen_buffer_t *Buffer, bitmap_t *Bitmap, float RealMinX, float RealMinY)
{
    int MinX = (int)(RealMinX + 0.5f);
    int MinY = (int)(RealMinY + 0.5f);

    int MaxX = MinX + Bitmap->Width;
    int MaxY = MinY + Bitmap->Height;

    int SrcOffsetX = 0;
    int SrcOffsetY = 0;

    if (MinX < 0) { SrcOffsetX = -MinX; MinX = 0; }
    if (MinY < 0) { SrcOffsetY = -MinY; MinY = 0; }

    if (MaxX > Buffer->Width) MaxX = Buffer->Width;
    if (MaxY > Buffer->Height) MaxY = Buffer->Height;

    unsigned char *Row = ((unsigned char *)Buffer->Memory + MinX * Buffer->BytesPerPixel + MinY * Buffer->Pitch);
    unsigned char *Src = (unsigned char *)Bitmap->Memory + SrcOffsetX * Bitmap->BytesPerPixel + SrcOffsetY * Bitmap->Pitch;

    for (int Y = MinY; Y < MaxY; ++Y)
    {
        unsigned int *Pixel = (unsigned int *)Row;
        unsigned int *SrcRow = (unsigned int *)Src;

        for (int X = MinX; X < MaxX; ++X)
        {
            *Pixel = BlendColor(*Pixel, *SrcRow);
            *Pixel++, *SrcRow++;
        }

        Src += Bitmap->Pitch;
        Row += Buffer->Pitch;
    }
}