#pragma once

#include <string>
#include "graphics.h"

typedef struct ascii_chars_t
{
    char Kern[256];

    char Advance;
    char Alignment;

    bitmap_t Bitmap;
} ascii_chars_t;

typedef struct true_type_t
{
    char LineSpace;
    ascii_chars_t Char[256];
} true_type_t;

class CFont
{
    private:
        true_type_t m_Chars;
    public:

        CFont(const std::string &FontName, float FontSize);
        ~CFont();
};