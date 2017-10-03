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
}

void CGround::Render(CGraphicsManager * GraphicsManager, game_offscreen_buffer_t * Buffer)
{
    float chunks = (float)Buffer->Width / (float)m_GroundBitmap->Width;
    int GroundChunks = (int)((int)chunks == chunks ? chunks : chunks + 1);

    for (int i = 0; i < GroundChunks; ++i)
    {
        GraphicsManager->DrawBitmap(Buffer, m_GroundBitmap, x + i * m_GroundBitmap->Width, y);
    }
}



