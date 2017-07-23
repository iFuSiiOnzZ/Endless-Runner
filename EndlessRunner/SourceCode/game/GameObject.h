#pragma once

#include "..\engine\graphics.h"
#include "..\engine\math.h"

struct IGameObject
{
    virtual void Update(float dt)                                                                  = 0;
    virtual void Render(CGraphicsManager * GraphicsRender, game_offscreen_buffer_t * Bufffer)      = 0;

    virtual Rect2d GetRectable()                                                                   = 0;
    virtual bool Collision(const Rect2d & CollisionRect)                                           = 0;
};