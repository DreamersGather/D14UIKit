#include "Common/Precompile.h"

#include "ConstraintLayout.h"

#include "Layout.h"
#include "Panel.h"

#include "UIKit/ConstraintLayout.h"

using namespace d14engine;

#define INIT_DIST_FIELD(Dst, Src, F_1, F_2) \
    if (Src.F_1.To##F_2.has_value()) \
    { \
        Dst.F_1.To##F_2 = \
        (float)Src.F_1.To##F_2.value(); \
    }
#define CONVERT_GEO_INFO(Dst_Info, Src_Info) do { \
    Dst_Info.keepWidth = Src_Info.keepWidth; \
    INIT_DIST_FIELD(Dst_Info, Src_Info, Left, Left); \
    INIT_DIST_FIELD(Dst_Info, Src_Info, Left, Right); \
    INIT_DIST_FIELD(Dst_Info, Src_Info, Right, Left); \
    INIT_DIST_FIELD(Dst_Info, Src_Info, Right, Right); \
    Dst_Info.keepHeight = Src_Info.keepHeight; \
    INIT_DIST_FIELD(Dst_Info, Src_Info, Top, Top); \
    INIT_DIST_FIELD(Dst_Info, Src_Info, Top, Bottom); \
    INIT_DIST_FIELD(Dst_Info, Src_Info, Bottom, Top); \
    INIT_DIST_FIELD(Dst_Info, Src_Info, Bottom, Bottom); \
} while (0)

namespace d14uikit
{
    _D14_UIKIT_LAYOUT_IMPL(ConstraintLayout)

    void ConstraintLayout::addElement(Panel* elem, const GeoInfo& geoInfo)
    {
        if (elem == nullptr) return;

        uikit::ConstraintLayout::GeometryInfo info = {};
        CONVERT_GEO_INFO(info, geoInfo);

        pimpl->uiobj->addElement(elem->getImpl()->uiobj, info);
    }

    void ConstraintLayout::removeElement(Panel* elem)
    {
        if (elem == nullptr) return;

        pimpl->uiobj->removeElement(elem->getImpl()->uiobj);
    }

    void ConstraintLayout::clearAllElements()
    {
        pimpl->uiobj->clearAllElements();
    }

    void ConstraintLayout::updateElement(Panel* elem, const GeoInfo& geoInfo)
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

#undef INIT_DIST_FIELD
#undef CONVERT_GEO_INFO
