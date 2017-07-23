#include "animation.h"
#include "macros.h"

CAnimation::CAnimation(const std::vector<animation_frame_t> &Frames) : m_CurrentIndex(0), m_CurrentTime(0.0f), m_TotalTime(0.0f)
{
    m_Frames = Frames;
    m_FrameEndTime.resize(m_Frames.size());

    for (size_t i = 0; i < m_Frames.size(); ++i)
    {
        m_TotalTime += m_Frames[i].duration;
        m_FrameEndTime[i] = m_TotalTime;
    }
}

CAnimation::CAnimation()
{
    m_CurrentIndex = 0;
    m_CurrentTime = m_TotalTime = 0.0f;
}

CAnimation::~CAnimation()
{
    m_FrameEndTime.clear();
    m_Frames.clear();

    m_CurrentIndex = 0;
    m_CurrentTime = 0.0f;
    m_TotalTime = 0.0f;
}

void CAnimation::SetFrames(const std::vector<animation_frame_t>& Frames)
{
    m_CurrentIndex = 0;
    m_CurrentTime = m_TotalTime = 0.0f;

    m_Frames = Frames;
    m_FrameEndTime.resize(m_Frames.size());

    for (size_t i = 0; i < m_Frames.size(); ++i)
    {
        m_TotalTime += m_Frames[i].duration;
        m_FrameEndTime[i] = m_TotalTime;
    }
}

void CAnimation::Update(float dt)
{
    m_CurrentTime += dt;

    if (m_CurrentTime >= m_TotalTime)
    {
        m_CurrentTime = std::fmod(m_CurrentTime, m_TotalTime);
        m_CurrentIndex = 0;
    }

    while (m_CurrentTime > m_FrameEndTime[m_CurrentIndex])
    {
        ++m_CurrentIndex;
    }
}

void CAnimation::Render(CGraphicsManager * GraphicsManager,  game_offscreen_buffer_t * Buffer, float x, float y)
{
    ASSERT(m_CurrentIndex < m_Frames.size(), "Frame index out of boundaries");
    GraphicsManager->DrawBitmap(Buffer, &m_Frames[m_CurrentIndex].bitmap, x, y);
}

animation_frame_t CAnimation::GetCurrentFrame()
{
    ASSERT(m_CurrentIndex < m_Frames.size(), "Frame index out of boundaries");
    return m_Frames[m_CurrentIndex];
}
