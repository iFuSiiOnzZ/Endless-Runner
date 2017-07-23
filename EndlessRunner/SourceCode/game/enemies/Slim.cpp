#include "Slim.h"

#include "..\..\engine\macros.h"
#include "..\..\engine\resource_manager.h"

CSlim::CSlim()
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

    m_Rect.position.x = 0.0f;
    m_Rect.position.y = 0.0f;

    m_Rect.size.x = 0.0f;
    m_Rect.size.y = 0.0f;
}

CSlim::~CSlim()
{
}

void CSlim::Update(float dt)
{
    if (m_IsAlive)
    {
        m_SlimAnimation.Update(dt);
        animation_frame_t frame = m_SlimAnimation.GetCurrentFrame();

        m_Rect.position.x = (float) frame.bitmap.Width;
        m_Rect.position.y = (float) frame.bitmap.Height;
    }
    else
    {
        m_Rect.position.x = (float) m_SlimDead->Width;
        m_Rect.position.y = (float) m_SlimDead->Height;
    }
}

void CSlim::Render(CGraphicsManager * GraphicsRender, game_offscreen_buffer_t * Buffer)
{
    if(m_IsAlive) m_SlimAnimation.Render(GraphicsRender, Buffer, 300.0f, 200.0f);
    else GraphicsRender->DrawBitmap(Buffer, m_SlimDead, 300.0f, 200.0f);
}

Rect2d CSlim::GetRectable()
{
    return Rect2d();
}

bool CSlim::Collision(const Rect2d & CollisionRect)
{
    Vec2f Boundaries =
    {
        CollisionRect.size.x + m_Rect.size.x,
        CollisionRect.size.y + m_Rect.size.y
    };

    Vec2f MyPos =
    {
        m_Rect.position.x + m_Rect.size.x * 0.5f,
        m_Rect.position.y + m_Rect.size.y * 0.5f
    };

    if (MyPos.x < CollisionRect.position.x || MyPos.x > Boundaries.x || MyPos.y < CollisionRect.position.x || MyPos.y > Boundaries.y) 
    {
        return false;
    }

    m_IsAlive = false;
    return true;
}
