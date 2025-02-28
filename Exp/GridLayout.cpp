#include "Common/Precompile.h"

#include "GridLayout.h"

#include "Layout.h"
#include "Panel.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/GridLayout.h"

using namespace d14engine;

#define CONVERT_GEO_INFO(Dst_Info, Src_Info) do { \
    Dst_Info.isFixedSize = Src_Info.fixedSize; \
    Dst_Info.axis.x = { (size_t)Src_Info.x.offset, (size_t)Src_Info.x.count }; \
    Dst_Info.axis.y = { (size_t)Src_Info.y.offset, (size_t)Src_Info.y.count }; \
    Dst_Info.spacing = \
    { \
        (float)Src_Info.spacing.left,  (float)Src_Info.spacing.top, \
        (float)Src_Info.spacing.right, (float)Src_Info.spacing.bottom \
    }; \
} while (0)

namespace d14uikit
{
    _D14_UIKIT_LAYOUT_IMPL(GridLayout)

    int GridLayout::horzCellCount() const
    {
        return (int)pimpl->uiobj->horzCellCount();
    }

    void GridLayout::setHorzCellCount(int value)
    {
        pimpl->uiobj->setCellCount(value, pimpl->uiobj->vertCellCount());
    }

    int GridLayout::vertCellCount() const
    {
        return (int)pimpl->uiobj->vertCellCount();
    }

    void GridLayout::setVertCellCount(int value)
    {
        pimpl->uiobj->setCellCount(pimpl->uiobj->horzCellCount(), value);
    }

    int GridLayout::horzMargin() const
    {
        return math_utils::round(pimpl->uiobj->horzMargin());
    }

    void GridLayout::setHorzMargin(int value)
    {
        pimpl->uiobj->setMargin((float)value, pimpl->uiobj->vertMargin());
    }

    int GridLayout::horzSpacing() const
    {
        return math_utils::round(pimpl->uiobj->horzSpacing());
    }

    void GridLayout::setHorzSpacing(int value)
    {
        pimpl->uiobj->setSpacing((float)value, pimpl->uiobj->vertSpacing());
    }

    int GridLayout::vertMargin() const
    {
        return math_utils::round(pimpl->uiobj->vertMargin());
    }

    void GridLayout::setVertMargin(int value)
    {
        pimpl->uiobj->setMargin(pimpl->uiobj->horzMargin(), (float)value);
    }

    int GridLayout::vertSpacing() const
    {
        return math_utils::round(pimpl->uiobj->vertSpacing());
    }

    void GridLayout::setVertSpacing(int value)
    {
        pimpl->uiobj->setSpacing(pimpl->uiobj->horzSpacing(), (float)value);
    }

    void GridLayout::addElement(Panel* elem, const GeoInfo& geoInfo)
    {
        if (elem == nullptr) return;

        uikit::GridLayout::GeometryInfo info = {};
        CONVERT_GEO_INFO(info, geoInfo);
        
        pimpl->uiobj->addElement(elem->getImpl()->uiobj, info);
    }

    void GridLayout::removeElement(Panel* elem)
    {
        if (elem == nullptr) return;

        pimpl->uiobj->removeElement(elem->getImpl()->uiobj);
    }

    void GridLayout::clearAllElements()
    {
        pimpl->uiobj->clearAllElements();
    }

    void GridLayout::updateElement(Panel* elem, const GeoInfo& geoInfo)
    {
        if (elem == nullptr) return;
        auto& uiobj = elem->getImpl()->uiobj;

        auto elemItor = pimpl->uiobj->findElement(uiobj);
        if (elemItor.has_value())
        {
            auto& info = elemItor.value()->second;
            CONVERT_GEO_INFO(info, geoInfo);

            pimpl->uiobj->updateElement(elemItor.value());
        }
    }
}

#undef CONVERT_GEO_INFO
