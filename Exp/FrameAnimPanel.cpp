#include "Common/Precompile.h"

#include "FrameAnimPanel.h"

#include "Image.h"
#include "Panel.h"

#include "UIKit/FrameAnimPanel.h"
#include "UIKit/ResourceUtils.h"

using namespace d14engine;

namespace d14uikit
{
    FrameAnimPanel::FrameAnimPanel() : FrameAnimPanel(Passkey{})
    {
        Panel::pimpl->uiobj =
        FrameAnimPanel::pimpl->uiobj =
        uikit::makeUIObject<uikit::FrameAnimPanel>(
            D2D1_RECT_F{},
            uikit::resource_utils::g_solidColorBrush);

        Panel::initialize();
        FrameAnimPanel::initialize();
    }

    FrameAnimPanel::FrameAnimPanel(Passkey)
        :
        Panel(Panel::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void FrameAnimPanel::initialize() { }

    void FrameAnimPanel::resetFrameState()
    {
        pimpl->uiobj->bitmapData.restore();
    }

    const std::vector<Image*>& FrameAnimPanel::frames() const
    {
        return pimpl->frames;
    }

    void FrameAnimPanel::setFrames(const std::vector<Image*>& images)
    {
        pimpl->frames = images;
        pimpl->uiobj->bitmapData.restore();

        auto& frames = pimpl->uiobj->bitmapData.fanim.frames;

        frames.resize(images.size());
        for (int i = 0; i < images.size(); ++i)
        {
            frames[i].bitmap = images[i]->getImpl()->bitmap;
            frames[i].opacity = pimpl->bitmapOpacity;
            frames[i].interpolationMode = (D2D1_INTERPOLATION_MODE)pimpl->bitmapInterpMode;
        }
    }

    float FrameAnimPanel::frameOpacity() const
    {
        return pimpl->bitmapOpacity;
    }

    void FrameAnimPanel::setFrameOpacity(float value)
    {
        pimpl->bitmapOpacity = value;

        for (auto& f : pimpl->uiobj->bitmapData.fanim.frames)
        {
            f.opacity = value;
        }
    }

    BitmapInterpMode FrameAnimPanel::frameInterpMode() const
    {
        return pimpl->bitmapInterpMode;
    }

    void FrameAnimPanel::setFrameInterpMode(BitmapInterpMode mode)
    {
        pimpl->bitmapInterpMode = mode;

        for (auto& f : pimpl->uiobj->bitmapData.fanim.frames)
        {
            f.interpolationMode = (D2D1_INTERPOLATION_MODE)mode;
        }
    }

    float FrameAnimPanel::frameTimeSpan() const
    {
        return pimpl->frameTimeSpan;
    }

    void FrameAnimPanel::setFrameTimeSpan(float secs)
    {
        pimpl->frameTimeSpan = secs;
        pimpl->uiobj->bitmapData.fanim.timeSpanDataInSecs = secs;
    }
}
