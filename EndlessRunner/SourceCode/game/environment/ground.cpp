#include "ground.h"

#include "..\..\engine\macros.h"
#include "..\..\engine\resource_manager.h"

CGround::CGround(float xx, float yy, float width) : x(xx), y(yy)
{
    CResourceManager *resource_manager = CResourceManager::GetInstance();
    m_GroundBitmap = resource_manager->Get<bitmap_t>("grass");

    float chunks = width / m_GroundBitmap->Width;
    m_GroundChunks = (int)(chunks + 0.5f);
}

void CGround::Update(float dt)
{
}

void CGround::Render(CGraphicsManager * GraphicsManager, game_offscreen_buffer_t * Buffer)
{
    for (int i = 0; i < m_GroundChunks; ++i)
    {
        GraphicsManager->DrawBitmap(Buffer, m_GroundBitmap, x + i * m_GroundBitmap->Width, y);
    }
}



