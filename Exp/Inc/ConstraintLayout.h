#pragma once

#include "Layout.h"
#include "ResizablePanel.h"

_D14_UIKIT_FWDEF(ConstraintLayout)

namespace d14uikit
{
    class DllExport ConstraintLayout : public ResizablePanel
    {
        _D14_UIKIT_PIMPL(ConstraintLayout)

        struct GeoInfo
        {
            bool keepWidth = false;

            struct HorzDistance
            {
                std::optional<int> ToLeft = std::nullopt;
                std::optional<int> ToRight = std::nullopt;
            }
            Left = {}, Right = {};

            bool keepHeight = false;

            struct VertDistance
            {
                std::optional<int> ToTop = std::nullopt;
                std::optional<int> ToBottom = std::nullopt;
            }
            Top = {}, Bottom = {};
        };
        _D14_UIKIT_LAYOUT_DECL(ConstraintLayout)
    };
}
