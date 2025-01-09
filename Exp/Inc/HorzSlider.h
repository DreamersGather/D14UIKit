#pragma once

#include "Panel.h"
#include "Slider.h"

_D14_UIKIT_FWDEF(HorzSlider)

namespace d14uikit
{
    class DllExport HorzSlider : public Panel
    {
        _D14_UIKIT_PIMPL(HorzSlider)
        _D14_UIKIT_SLIDER_DECL(HorzSlider)
    };
}
