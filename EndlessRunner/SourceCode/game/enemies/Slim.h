#pragma once

#include "enemy.h"
#include "..\..\engine\animation.h"

class CSlim : public CEnemy
{
    private:
        CAnimation m_SlimAnimation;
        bitmap_t *m_SlimDead;

        bool m_IsAlive;
        Rect2d m_Rect;

    public:
        CSlim();
        ~CSlim();

        void Update(float dt);
        void Render(CGraphicsManager * GraphicsRender, game_offscreen_buffer_t * Buffer) ;

        Rect2d GetRectable();
        bool Collision(const Rect2d & CollisionRect);
};