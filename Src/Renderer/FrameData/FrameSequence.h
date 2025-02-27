#pragma once

#include "Common/Precompile.h"

#include "Renderer/FrameData/FrameAnimation.h"
#include "Renderer/Renderer.h"
#include "Renderer/TickTimer.h"

namespace d14engine::renderer
{
    template<typename FrameType>
    struct FrameSequence
    {
        using FrameAnim = FrameAnimation<FrameType>;

        using FrameArray = FrameAnim::FrameArray;
        using FramePackage = FrameAnim::FramePackage;

        bool enabled = true;
        bool visible = true;

        FrameAnim fanim = {};

        void restore()
        {
            fanim.restore();
        }

        void update(Renderer* rndr)
        {
            if (enabled)
            {
                fanim.update((float)rndr->timer()->deltaSecs());
            }
        }
    };
}
