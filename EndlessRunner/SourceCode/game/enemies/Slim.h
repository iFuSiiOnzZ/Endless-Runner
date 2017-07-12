#pragma once

#include "enemy.h"
#include "..\..\engine\animation.h"

class CSlim : public CEnemy
{
    private:
        CAnimation m_SlimAnimation;

    public:
        CSlim();
        ~CSlim();

        void Update(float dt);
        void Render(CGraphicsManager * GraphicsRender, game_offscreen_buffer_t * Bufffer) ;

        rect2d GetRectable();
        bool Collision(rect2d CollisionRect);
};