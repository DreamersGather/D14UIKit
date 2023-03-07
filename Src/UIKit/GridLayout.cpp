#include "Common/Precompile.h"

#include "UIKit/GridLayout.h"

namespace d14engine::uikit
{
    GridLayout::GridLayout(
        size_t horzCellCount,
        size_t vertCellCount,
        float horzSpacing,
        float vertSpacing,
        const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        Layout(rect),
        m_horzCellCount(horzCellCount),
        m_vertCellCount(vertCellCount),
        m_horzSpacing(horzSpacing),
        m_vertSpacing(vertSpacing) { }

    float GridLayout::horzSpacing() const
    {
        return m_horzSpacing;
    }

    float GridLayout::vertSpacing() const
    {
        return m_vertSpacing;
    }

    void GridLayout::setSpacing(float horz, float vert)
    {
        m_horzSpacing = horz;
        m_vertSpacing = vert;

        updateAllElements();
    }

    size_t GridLayout::horzCellCount() const
    {
        return m_horzCellCount;
    }

    size_t GridLayout::vertCellCount() const
    {
        return m_vertCellCount;
    }

    void GridLayout::setCellCount(size_t horz, size_t vert)
    {
        m_horzCellCount = std::max(horz, 1ull);
        m_vertCellCount = std::max(vert, 1ull);

        updateAllElements();
    }

    void GridLayout::updateElement(ShrdPtrParam<Panel> elem, const GeometryInfo& geoInfo)
    {
        float deltaX = width()  / m_horzCellCount;
        float deltaY = height() / m_vertCellCount;

        if (geoInfo.isFixedSize)
        {
            float occupiedWidth  = geoInfo.axis.x.count * deltaX;
            float occupiedHeight = geoInfo.axis.y.count * deltaY;

            float leftOffset = (occupiedWidth  - elem->width())  * 0.5f;
            float topOffset  = (occupiedHeight - elem->height()) * 0.5f;

            elem->move
            (
                std::round(geoInfo.axis.x.offset * deltaX + leftOffset),
                std::round(geoInfo.axis.y.offset * deltaY + topOffset )
            );
        }
        else // variable size
        {
            float leftOffset = m_horzSpacing + geoInfo.spacing.left;
            float topOffset  = m_vertSpacing + geoInfo.spacing.top ;

            float horzSpacing = 2.0f * (m_horzSpacing + geoInfo.spacing.left + geoInfo.spacing.right);
            float vertSpacing = 2.0f * (m_vertSpacing + geoInfo.spacing.top + geoInfo.spacing.bottom);

            elem->transform
            (
                std::round(geoInfo.axis.x.offset * deltaX + leftOffset ),
                std::round(geoInfo.axis.y.offset * deltaY + topOffset  ),
                std::round(geoInfo.axis.x.count  * deltaX - horzSpacing),
                std::round(geoInfo.axis.y.count  * deltaY - vertSpacing)
            );
        }
    }
}
