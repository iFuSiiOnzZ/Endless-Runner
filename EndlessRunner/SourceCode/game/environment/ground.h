#pragma once

#include "..\..\engine\graphics.h"

class CGround
{
    private:
        bitmap_t *m_GroundBitmap;
        float x, y;

    public:
        CGround(float x, float y);

        void Update(float dt);
        void Render(CGraphicsManager *GraphicsManager, game_offscreen_buffer_t * Buffer);
};