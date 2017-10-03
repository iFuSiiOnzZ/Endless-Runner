#include "Font.h"
#include "utils.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <stdarg.h>

CFont::CFont(const std::string & FontName, float FontSize)
{
    int Ascent = 0;
    int Descent = 0;
    int LineSpace = 0;

    stbtt_fontinfo StbFont = { 0 };
    file_in_memory f = GetFileContent(FontName.c_str());

    stbtt_InitFont(&StbFont, (unsigned char *)f.Memory, stbtt_GetFontOffsetForIndex((unsigned char *)f.Memory, 0));
    stbtt_GetFontVMetrics(&StbFont, &Ascent, &Descent, &LineSpace);

    float FontScale = stbtt_ScaleForPixelHeight(&StbFont, FontSize);
    int BaseLine = (int)(FontScale * Ascent);

    memset(&m_Font, 0, sizeof(true_type_t));
    m_Font.LineSpace = (int)((FontScale * LineSpace) + 0.5f);

    for (unsigned int i = 0; i < 256; ++i)
    {
        int w = 0, h = 0;
        unsigned char *bitmap = stbtt_GetCodepointBitmap(&StbFont, 0.0f, FontScale, (unsigned char)i, &w, &h, 0, 0);

        int ix0 = 0, iy0 = 0, ix1 = 0, iy1 = 0;
        int Advance = 0, lsb = 0, Kern = 0;

        stbtt_GetCodepointHMetrics(&StbFont, (unsigned char)i, &Advance, &lsb);
        stbtt_GetCodepointBitmapBox(&StbFont, (unsigned char)i, 0.0f, FontScale, &ix0, &iy0, &ix1, &iy1);

        for (unsigned int j = 0; j < 256; ++j)
        {
            Kern = stbtt_GetCodepointKernAdvance(&StbFont, (unsigned char)i, (unsigned char)j);
            m_Font.Char[i].Kern[j] = (int)(FontScale * Kern);
        }

        m_Font.Char[i].Bitmap.Width = w;
        m_Font.Char[i].Bitmap.Height = h;

        m_Font.Char[i].Bitmap.BytesPerPixel = 4;
        m_Font.Char[i].Bitmap.Pitch = w * m_Font.Char[i].Bitmap.BytesPerPixel;

        m_Font.Char[i].Alignment = BaseLine + iy0;
        m_Font.Char[i].Advance = (int)(Advance * FontScale);

        if (bitmap == NULL) continue;
        m_Font.Char[i].Bitmap.Memory = malloc(w * h * m_Font.Char[i].Bitmap.BytesPerPixel);

        unsigned int *DestRow = (unsigned int *)m_Font.Char[i].Bitmap.Memory;
        unsigned char *Source = bitmap;

        for (int i = 0; i < w * h; ++i)
        {
            unsigned char Alpha = *Source++;
            *DestRow++ = (Alpha << 24) | (Alpha << 16) | (Alpha << 8) | (Alpha << 0);
        }

        stbtt_FreeBitmap(bitmap, NULL);
    }

    ReleaseFileContent(&f);
}

CFont::~CFont()
{
    for (size_t i = 0; i < 256; ++i) free(m_Font.Char[i].Bitmap.Memory);
    memset(&m_Font, 0, sizeof(true_type_t));
}

unsigned int CFont::DrawString(CGraphicsManager *GraphicsManager, game_offscreen_buffer_t *Buffer, int x, int y, float r, float g, float b, char *Format, ...)
{
    int MaxLineHeight = 0, NewLineHeight = 0;
    int CarretPos = x;

    va_list args;
    char AuxText[1024 * 8]; // NOTE(Andrei): Not to sure about the size

    va_start(args, Format);
    vsnprintf(AuxText, 1024 * 8, Format, args);

    for (char *c = AuxText; *c != '\0'; ++c)
    {
        ascii_chars_t *Char = &m_Font.Char[*c];
        int ChartStartAt = 0;

        if (CarretPos + Char->Bitmap.Width >= Buffer->Width)
        {
            NewLineHeight += MaxLineHeight + m_Font.LineSpace;
            CarretPos = x;
        }

        int MinX = CarretPos;
        int MinY = y + Char->Alignment;

        int MaxX = MinX + Char->Bitmap.Width;
        int MaxY = MinY + Char->Bitmap.Height;

        if (MinX < 0) MinX = 0;
        if (MaxX > Buffer->Width) MaxX = Buffer->Width;

        if (MinY < 0) { ChartStartAt = abs(MinY); MinY += ChartStartAt; }
        if (MaxY > Buffer->Height) MaxY = Buffer->Height;

        CarretPos += Char->Advance + Char->Kern[*(unsigned char *)((uintptr_t)c + 1)];
        int CharSize = Char->Bitmap.Height + Char->Alignment;
        MaxLineHeight = CharSize > MaxLineHeight ? CharSize : MaxLineHeight;

        if (MaxY <= 0 || MinY + NewLineHeight >= Buffer->Height)
        {
            continue;
        }

        unsigned char *DstRow = ((unsigned char *)Buffer->Memory) + MinX * Buffer->BytesPerPixel + ((ChartStartAt ? 0 : MinY) + NewLineHeight) * Buffer->Pitch;
        unsigned char *SrcRow = ((unsigned char *)Char->Bitmap.Memory) + ChartStartAt * Char->Bitmap.Pitch;

        if (Buffer->Height - (MinY + NewLineHeight) < Char->Bitmap.Height)
        {
            MinY += Char->Bitmap.Height - (Buffer->Height - (MinY + NewLineHeight));
        }

        for (int Y = MinY; Y < MaxY; ++Y)
        {
            unsigned int *Dst = (unsigned int *)DstRow;
            unsigned int *Src = (unsigned int *)SrcRow;

            for (int X = MinX; X < MaxX; ++X)
            {
                unsigned char s = (*Src++ & 0xFF);
                *Dst++ = 255 << 24 | (unsigned char)(s * r) << 16 | (unsigned char)(s * g) << 8 | (unsigned char)(s * b) << 0;
            }

            DstRow += Buffer->Pitch;
            SrcRow += Char->Bitmap.Pitch;
        }
    }

    return NewLineHeight + (MaxLineHeight + m_Font.LineSpace);
}
