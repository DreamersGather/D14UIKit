#pragma once

#include "Panel.h"
#include "Slider.h"

namespace d14uikit
{
    class DllExport VertSlider : public Panel
    {
        _D14_UIKIT_PIMPL(VertSlider)

        VertSlider();

        _D14_UIKIT_SLIDER_DECL

        struct Callback
        {
            std::function<void(VertSlider*, float)> onValueChange = {};

            std::function<void(VertSlider*, float)> onEndSliding = {};

            std::function<void(VertSlider*, float)> onStartSliding = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onValueChange(float value);

        virtual void onEndSliding(float value);

        virtual void onStartSliding(float value);
    };
}
