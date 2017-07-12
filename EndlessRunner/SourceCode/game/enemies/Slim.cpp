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

    /*bmp = resource_manager->Get<bitmap_t>("slimeDead");
    ASSERT(bmp != nullptr, "resource not found");

    frame.bitmap = *bmp; frame.duration = 0.16f;
    frames.push_back(frame);*/

    m_SlimAnimation.SetFrames(frames);
}

CSlim::~CSlim()
{
}

void CSlim::Update(float dt)
{
    m_SlimAnimation.Update(dt);
}

void CSlim::Render(CGraphicsManager * GraphicsRender, game_offscreen_buffer_t * Bufffer)
{
    m_SlimAnimation.Render(GraphicsRender, Bufffer, 300.0f, 200.0f);
}

rect2d CSlim::GetRectable()
{
    return rect2d();
}

bool CSlim::Collision(rect2d CollisionRect)
{
    return false;
}
