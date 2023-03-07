#pragma once

#include "Common/Precompile.h"

#include "UIKit/Layout.h"

namespace d14engine::uikit
{
    struct ConstraintLayoutGeometryInfo
    {
        bool keepWidth = true;

        struct HorzDistance
        {
            Optional<float> ToLeft = std::nullopt, ToRight = std::nullopt;
        }
        Left = {}, Right = {};

        bool keepHeight = true;

        struct VertDistance
        {
            Optional<float> ToTop = std::nullopt, ToBottom = std::nullopt;
        }
        Top = {}, Bottom = {};
    };

    struct ConstraintLayout : Layout<ConstraintLayoutGeometryInfo>
    {
        explicit ConstraintLayout(const D2D1_RECT_F& rect = {});

    public:
        using Layout::updateElement;

    protected:
        void updateElement(ShrdPtrParam<Panel> elem, const GeometryInfo& geoInfo) override;
    };
}
