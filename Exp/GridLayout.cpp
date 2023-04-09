﻿#include "Common/Precompile.h"

#include "GridLayout.h"

#include "Panel.h"
#include "ResizablePanel.h"

#include "Common/MathUtils/Basic.h"

#include "UIKit/GridLayout.h"

using namespace d14engine;

#define CONVERT_GEO_INFO(Dst_Info, Src_Info) do { \
    info.isFixedSize = geoInfo.fixedSize; \
    info.axis.x = { (size_t)geoInfo.x.offset, (size_t)geoInfo.x.count }; \
    info.axis.y = { (size_t)geoInfo.y.offset, (size_t)geoInfo.y.count }; \
    info.spacing = \
    { \
        (float)geoInfo.spacing.left,  (float)geoInfo.spacing.top, \
        (float)geoInfo.spacing.right, (float)geoInfo.spacing.bottom \
    }; \
} while (0)

namespace d14uikit
{
    GridLayout::GridLayout()
        :
        GridLayout(Passkey{})
    {
        Panel::pimpl->uiobj =
        ResizablePanel::pimpl->uiobj =
        GridLayout::pimpl->uiobj =
        uikit::makeUIObject<uikit::GridLayout>();

        Panel::initialize();
        ResizablePanel::initialize();
        GridLayout::initialize();
    }

    GridLayout::GridLayout(Passkey)
        :
        Panel(Panel::Passkey{}),
        ResizablePanel(ResizablePanel::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void GridLayout::initialize() { }

    int GridLayout::horzSpacing() const
    {
        return math_utils::round(pimpl->uiobj->horzSpacing());
    }

    void GridLayout::setHorzSpacing(int value)
    {
        pimpl->uiobj->setSpacing((float)value, pimpl->uiobj->vertSpacing());
    }

    int GridLayout::vertSpacing() const
    {
        return math_utils::round(pimpl->uiobj->vertSpacing());
    }

    void GridLayout::setVertSpacing(int value)
    {
        pimpl->uiobj->setSpacing(pimpl->uiobj->horzSpacing(), (float)value);
    }

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
