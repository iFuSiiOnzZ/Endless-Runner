#pragma once

#include "..\..\engine\stb_image.h"
#include "..\..\engine\graphics.h"
#include "..\..\engine\macros.h"

struct CContentLoader
{
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

        ///////////////////////////////////////////////////////////////////////

        bmp = new bitmap_t;
        resource_manager->Add<bitmap_t>("sun", bmp);

        bmp->Memory = stbi_load("..\\assets\\sun.png", &bmp->Width, &bmp->Height, &bmp->BytesPerPixel, 0);
        bmp->Pitch = bmp->Width * bmp->BytesPerPixel;
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
    }
};