#include "ground.h"

#include "..\..\engine\macros.h"
#include "..\..\engine\resource_manager.h"

CGround::CGround(float xx, float yy) : x(xx), y(yy)
{
    CResourceManager *resource_manager = CResourceManager::GetInstance();
    m_GroundBitmap = resource_manager->Get<bitmap_t>("grass");
}

void CGround::Update(float dt)
{
    x -= 10.0f * dt;

    if (x <= -m_GroundBitmap->Width)
    {
        x = 0.0f;
    }
}

void CGround::Render(CGraphicsManager * GraphicsManager, game_offscreen_buffer_t * Buffer)
{
    GraphicsManager->DrawBitmap(Buffer, m_GroundBitmap, x, y);
}



