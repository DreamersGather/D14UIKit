#pragma once

#include "Common/Precompile.h"

#include "UIKit/Layout.h"

namespace d14engine::uikit
{
    struct GridLayoutGeometryInfo
    {
        bool isFixedSize = false;

        struct Axis
        {
            struct Data
            {
                size_t offset = {}, count = {};
            }
            x = {}, y = {};
        }
        axis = {};

        D2D1_RECT_F spacing = math_utils::zeroRectF();
    };

    struct GridLayout : Layout<GridLayoutGeometryInfo>
    {
        GridLayout(
            size_t horzCellCount = 1,
            size_t vertCellCount = 1,
            float horzMargin = 0.0f,
            float horzSpacing = 0.0f,
            float vertMargin = 0.0f,
            float vertSpacing = 0.0f,
            const D2D1_RECT_F& rect = {});

    protected:
        float m_horzCellDelta = {}, m_vertCellDelta = {};
        size_t m_horzCellCount = {}, m_vertCellCount = {};

        void updateCellDeltaInfo();

    public:
        float horzCellDelta() const;
        size_t horzCellCount() const;

        float vertCellDelta() const;
        size_t vertCellCount() const;

        void setCellCount(size_t horz, size_t vert);

    protected:
        float m_horzMargin = {}, m_vertMargin = {};
        float m_horzSpacing = {}, m_vertSpacing = {};

    public:
        float horzMargin() const;
        float horzSpacing() const;

        float vertMargin() const;
        float vertSpacing() const;

        void setMargin(float horz, float vert);
        void setSpacing(float horz, float vert);

    public:
        using Layout::updateElement;

    protected:
        void updateElement(ShrdPtrParam<Panel> elem, const GeometryInfo& geoInfo) override;

    protected:
        // Panel
        void onSizeHelper(SizeEvent& e) override;
    };
}
