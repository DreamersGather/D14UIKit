#include "Common/Precompile.h"

#include "UIKit/ConstraintLayout.h"

namespace d14engine::uikit
{
    ConstraintLayout::ConstraintLayout(const D2D1_RECT_F& rect)
        :
        Panel(rect, resource_utils::g_solidColorBrush),
        Layout(rect) { }

    void ConstraintLayout::updateElement(ShrdPtrParam<Panel> elem, const GeometryInfo& geoInfo)
    {
        D2D1_RECT_F rect = elem->relativeRect();

        // Left <------------------------ Left ------------------------> Right
        if (geoInfo.Left.ToLeft.has_value())
        {
            rect.left = geoInfo.Left.ToLeft.value();

            if (geoInfo.keepWidth)
            {
                rect.right = rect.left + elem->width();
            }
        }
        else if (geoInfo.Left.ToRight.has_value())
        {
            rect.left = width() - geoInfo.Left.ToRight.value();

            if (geoInfo.keepWidth)
            {
                rect.right = rect.left + elem->width();
            }
        }
        // Left <------------------------ Right -----------------------> Right
        if (geoInfo.Right.ToLeft.has_value())
        {
            rect.right = geoInfo.Right.ToLeft.value();

            if (geoInfo.keepWidth)
            {
                rect.left = rect.right - elem->width();
            }
        }
        else if (geoInfo.Right.ToRight.has_value())
        {
            rect.right = width() - geoInfo.Right.ToRight.value();

            if (geoInfo.keepWidth)
            {
                rect.left = rect.right - elem->width();
            }
        }
        // Top <------------------------- Top ------------------------> Bottom
        if (geoInfo.Top.ToTop.has_value())
        {
            rect.top = geoInfo.Top.ToTop.value();

            if (geoInfo.keepHeight)
            {
                rect.bottom = rect.top + elem->height();
            }
        }
        else if (geoInfo.Top.ToBottom.has_value())
        {
            rect.top = height() - geoInfo.Top.ToBottom.value();

            if (geoInfo.keepHeight)
            {
                rect.bottom = rect.top + elem->height();
            }
        }
        // Top <------------------------- Bottom ---------------------> Bottom
        if (geoInfo.Bottom.ToTop.has_value())
        {
            rect.bottom = geoInfo.Bottom.ToTop.value();

            if (geoInfo.keepHeight)
            {
                rect.top = rect.bottom - elem->height();
            }
        }
        else if (geoInfo.Bottom.ToBottom.has_value())
        {
            rect.bottom = height() - geoInfo.Bottom.ToBottom.value();

            if (geoInfo.keepHeight)
            {
                rect.top = rect.bottom - elem->height();
            }
        }
        elem->transform(rect);
    }
}
