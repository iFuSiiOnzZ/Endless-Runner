#pragma once

#include "..\..\engine\stb_image.h"
#include "..\..\engine\graphics.h"
#include "..\..\engine\macros.h"

struct CContentLoader
{
    static void from3To4BPX(bitmap_t *bmp)
    {
        size_t bmp_size = bmp->Width * bmp->Height * 4;
        char *img = (char *)malloc(bmp_size);

        ASSERT(img != nullptr, "img has to point to a valid memory");
        bmp->BytesPerPixel = 4;

        for (size_t i = 0; i < bmp->Width * bmp->Height; ++i)
        {
            img[i * 4 + 0] = ((char *)bmp->Memory)[i * 3 + 0];
            img[i * 4 + 1] = ((char *)bmp->Memory)[i * 3 + 1];
            img[i * 4 + 2] = ((char *)bmp->Memory)[i * 3 + 2];
            img[i * 4 + 3] = (char)255;
        }

        stbi_image_free(bmp->Memory);
        bmp->Memory = img;
    }

    static void bgr2rgb(bitmap_t *bmp)
    {
        int *bitmap = (int *) bmp->Memory;

        for (size_t i = 0; i < bmp->Width * bmp->Height; ++i)
        {
            bitmap[i] = (bitmap[i] & 0xFF000000) | (bitmap[i] & 0xFF0000) >> 16 | (bitmap[i] & 0xFF00) | (bitmap[i] & 0xFF) << 16;
        }
    }

    static void Load()
    {
        CResourceManager *resource_manager = CResourceManager::GetInstance();
        resource_manager->Register<bitmap_t>();
        bitmap_t *bmp = nullptr;

        ///////////////////////////////////////////////////////////////////////

        bmp = new bitmap_t;
        resource_manager->Add<bitmap_t>("tree", bmp);

        bmp->Memory = stbi_load("..\\assets\\tree2.png", &bmp->Width, &bmp->Height, &bmp->BytesPerPixel, 0);
        bmp->Pitch = bmp->Width * bmp->BytesPerPixel;
        bgr2rgb(bmp);

        ///////////////////////////////////////////////////////////////////////

        bmp = new bitmap_t;
        resource_manager->Add<bitmap_t>("sun", bmp);

        bmp->Memory = stbi_load("..\\assets\\sun.png", &bmp->Width, &bmp->Height, &bmp->BytesPerPixel, 0);
        bmp->Pitch = bmp->Width * bmp->BytesPerPixel;
        bgr2rgb(bmp);

        ///////////////////////////////////////////////////////////////////////

        bmp = new bitmap_t;
        resource_manager->Add<bitmap_t>("grass", bmp);

        bmp->Memory = stbi_load("..\\assets\\grass.png", &bmp->Width, &bmp->Height, &bmp->BytesPerPixel, 0);
        if (bmp->BytesPerPixel == 3) from3To4BPX(bmp);
        bmp->Pitch = bmp->Width * bmp->BytesPerPixel;
        bgr2rgb(bmp);
    }

    static void UnLoad()
    {
        CResourceManager *resource_manager = CResourceManager::GetInstance();
        bitmap_t *bmp = nullptr;

        ///////////////////////////////////////////////////////////////////////

        bmp = resource_manager->Remove<bitmap_t>("tree");
        ASSERT(bmp != nullptr, "resource not fount");

        stbi_image_free(bmp->Memory);
        delete bmp;

        ///////////////////////////////////////////////////////////////////////

        bmp = resource_manager->Remove<bitmap_t>("sun");
        ASSERT(bmp != nullptr, "resource not fount");

        stbi_image_free(bmp->Memory);
        delete bmp;

        ///////////////////////////////////////////////////////////////////////

        bmp = resource_manager->Remove<bitmap_t>("grass");
        ASSERT(bmp != nullptr, "resource not fount");

        stbi_image_free(bmp->Memory);
        delete bmp;
    }
};