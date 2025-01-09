#pragma once

#include "Panel.h"
#include "Slider.h"

_D14_UIKIT_FWDEF(VertSlider)

namespace d14uikit
{
    class DllExport VertSlider : public Panel
    {
        _D14_UIKIT_PIMPL(VertSlider)
        _D14_UIKIT_SLIDER_DECL(VertSlider)
    };
}
