#pragma once

#include "graphics.h"
#include <vector>

typedef struct animation_frame_t
{
    bitmap_t bitmap;
    float duration;
} animation_frame_t;

class CAnimation
{
    private:
        std::vector<animation_frame_t> m_Frames;
        std::vector<float> m_FrameEndTime;

        unsigned int m_CurrentIndex;
        float m_CurrentTime;
        float m_TotalTime;

    public:
        CAnimation();
        ~CAnimation();

        CAnimation(const std::vector<animation_frame_t> &Frames);
        void SetFrames(const std::vector<animation_frame_t> &Frames);

        void Update(float dt);
        void Render(CGraphicsManager *GraphicsManager, game_offscreen_buffer_t * Buffer, float x, float y);
};
