#pragma once

#include "Panel.h"
#include "Slider.h"

namespace d14uikit
{
    class DllExport HorzSlider : public Panel
    {
        _D14_UIKIT_PIMPL(HorzSlider)

        HorzSlider();

        _D14_UIKIT_SLIDER_DECL

        struct Callback
        {
            std::function<void(HorzSlider*, float)> onValueChange = {};

            std::function<void(HorzSlider*, float)> onEndSliding = {};

            std::function<void(HorzSlider*, float)> onStartSliding = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onValueChange(float value);

        virtual void onEndSliding(float value);

        virtual void onStartSliding(float value);
    };
}
