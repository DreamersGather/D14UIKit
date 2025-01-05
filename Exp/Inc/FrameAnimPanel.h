#pragma once

#include "Panel.h"

namespace d14uikit
{
    class DllExport FrameAnimPanel : public Panel
    {
        _D14_UIKIT_PIMPL(FrameAnimPanel)

        FrameAnimPanel();

        void resetFrameState();

        const std::vector<Image*>& frames() const;
        void setFrames(const std::vector<Image*>& images);

        float frameOpacity() const;
        void setFrameOpacity(float value);

        BitmapInterpMode frameInterpMode() const;
        void setFrameInterpMode(BitmapInterpMode mode);

        float frameTimeSpan() const;
        void setFrameTimeSpan(float secs);
    };
}
