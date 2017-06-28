#include "Font.h"
#include "utils.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

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

    memset(&m_Chars, 0, sizeof(true_type_t));
    m_Chars.LineSpace = (int)((FontScale * LineSpace) + 0.5f);

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
            m_Chars.Char[i].Kern[j] = (int)(FontScale * Kern);
        }

        m_Chars.Char[i].Bitmap.Width = w;
        m_Chars.Char[i].Bitmap.Height = h;

        m_Chars.Char[i].Bitmap.BytesPerPixel = 4;
        m_Chars.Char[i].Bitmap.Pitch = w * m_Chars.Char[i].Bitmap.BytesPerPixel;

        m_Chars.Char[i].Alignment = BaseLine + iy0;
        m_Chars.Char[i].Advance = (int)(Advance * FontScale);

        if (bitmap == NULL) continue;
        m_Chars.Char[i].Bitmap.Memory = malloc(w * h * m_Chars.Char[i].Bitmap.BytesPerPixel);

        unsigned int *DestRow = (unsigned int *)m_Chars.Char[i].Bitmap.Memory;
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
    for (size_t i = 0; i < 256; ++i) free(m_Chars.Char[i].Bitmap.Memory);
    memset(&m_Chars, 0, sizeof(true_type_t));
}
