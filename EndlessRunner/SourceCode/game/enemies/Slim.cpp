/*
    Project includes
*/
#include "Slim.h"

#include "..\..\engine\macros.h"
#include "..\..\engine\resource_manager.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/
#include <Windows.h>


CSlim::CSlim(float x, float y)
{
    CResourceManager *resource_manager = CResourceManager::GetInstance();
    std::vector<animation_frame_t> frames;
    animation_frame_t frame;

    bitmap_t *bmp = resource_manager->Get<bitmap_t>("slimeWalk1");
    ASSERT(bmp != nullptr, "resource not found");

    frame.bitmap = *bmp; frame.duration = 0.36f;
    frames.push_back(frame);

    bmp = resource_manager->Get<bitmap_t>("slimeWalk2");
    ASSERT(bmp != nullptr, "resource not found");

    frame.bitmap = *bmp; frame.duration = 0.36f;
    frames.push_back(frame);

    m_SlimDead = resource_manager->Get<bitmap_t>("slimeDead");
    ASSERT(bmp != nullptr, "resource not found");

    m_SlimAnimation.SetFrames(frames);
    m_IsAlive = true;

    m_Position.x = m_Rect.UpperLeft.x = x;
    m_Position.y = m_Rect.UpperLeft.y = y;

    m_Rect.RightBottom.x = 0.0f;
    m_Rect.RightBottom.y = 0.0f;

    m_Velocity.x = -10.0f;
    m_Velocity.y = 0.0f;
}

CSlim::~CSlim()
{
}

void CSlim::Update(float dt)
{
    Vec2f Velocity = m_Velocity * dt;
    m_Position = m_Position - Velocity;

    m_Rect.UpperLeft.x = m_Position.x;
    m_Rect.UpperLeft.y = m_Position.y;

    if (m_IsAlive)
    {
        m_SlimAnimation.Update(dt);
        animation_frame_t frame = m_SlimAnimation.GetCurrentFrame();

        m_Rect.RightBottom.x = (float) frame.bitmap.Width  + m_Rect.UpperLeft.x;
        m_Rect.RightBottom.y = (float) frame.bitmap.Height + m_Rect.UpperLeft.y;
    }
    else
    {
        m_Rect.RightBottom.x = (float) m_SlimDead->Width  + m_Rect.UpperLeft.x;
        m_Rect.RightBottom.y = (float) m_SlimDead->Height + m_Rect.UpperLeft.y;
    }
}

void CSlim::Render(CGraphicsManager * GraphicsRender, game_offscreen_buffer_t * Buffer)
{
    if(m_IsAlive) m_SlimAnimation.Render(GraphicsRender, Buffer, m_Position.x, m_Position.y);
    else GraphicsRender->DrawBitmap(Buffer, m_SlimDead, m_Position.x, m_Position.y);
}

Rect2d CSlim::GetRectable()
{
    return m_Rect;
}

bool CSlim::Collision(const Rect2d & CollisionRect)
{
    Vec2f Boundaries =
    {
        CollisionRect.RightBottom.x + m_Rect.RightBottom.x,
        CollisionRect.RightBottom.y + m_Rect.RightBottom.y
    };

    Vec2f MyPos =
    {
        m_Rect.UpperLeft.x + m_Rect.RightBottom.x * 0.5f,
        m_Rect.UpperLeft.y + m_Rect.RightBottom.y * 0.5f
    };

    if (MyPos.x < CollisionRect.UpperLeft.x || MyPos.x > Boundaries.x || MyPos.y < CollisionRect.UpperLeft.x || MyPos.y > Boundaries.y) 
    {
        return false;
    }

    m_IsAlive = false;
    return true;
}
