#pragma once

#include "enemy.h"

class CSlim : public CEnemy
{
    private:

    public:
        void Update(float dt);
        void Render(CGraphicsManager * GraphicsRender, game_offscreen_buffer_t * Bufffer) ;

        rect2d GetRectable();
        bool Collision(rect2d CollisionRect);
};