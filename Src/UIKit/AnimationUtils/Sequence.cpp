#include "Common/Precompile.h"

#include "UIKit/AnimationUtils/Sequence.h"

#include "Renderer/Renderer.h"
#include "Renderer/TickTimer.h"

#include "UIKit/BitmapObject.h"

using namespace d14engine::renderer;

namespace d14engine::uikit::animation_utils
{
    void DynamicBitmap::restore()
    {
        m_currFrameIndex = 0;
        m_prevFrameElapsedSecs = 0.0f;
    }

    void DynamicBitmap::update(Renderer* rndr)
    {
        if (!frames.empty())
        {
            float ts = 0.0f;

            if (equalTimeSpanOptimization.enabled)
            {
                ts = equalTimeSpanOptimization.durationInSecs;
            }
            else if (frames.size() == timeSpansInSecs.size())
            {
                if (m_currFrameIndex >= 0 && m_currFrameIndex < frames.size())
                {
                    ts = timeSpansInSecs[m_currFrameIndex];
                }
            }
            else // Both control modes are invalid.
            {
                m_currFrameIndex = SIZE_MAX;
                return;
            }
            m_prevFrameElapsedSecs += (float)rndr->timer()->deltaSecs();

            if (m_prevFrameElapsedSecs >= ts)
            {
                m_currFrameIndex = (m_currFrameIndex + 1) % frames.size();
                m_prevFrameElapsedSecs = 0.0f;
            }
        }
        else m_currFrameIndex = SIZE_MAX;
    }

    void DynamicBitmap::draw(Renderer* rndr)
    {
        if (visible && m_currFrameIndex >= 0 && m_currFrameIndex < frames.size())
        {
            rndr->d2d1DeviceContext()->DrawBitmap(
                frames[m_currFrameIndex].Get(), rect,
                opacity, BitmapObject::g_interpolationMode);
        }
    }
}
