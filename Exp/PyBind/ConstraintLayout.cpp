#include "Common/Precompile.h"

#include "ConstraintLayout.h"

#include "Layout.h"
#include "ResizablePanel.h"

#include "Inc/ConstraintLayout.h"

namespace d14uikit
{
    void Py_InitConstraintLayout(py::module_& m)
    {
        py::class_<ConstraintLayout, ResizablePanel,
            PyResizablePanel<ConstraintLayout>> i(m, "ConstraintLayout");

        _D14_UIKIT_PYBIND_LAYOUT(ConstraintLayout)

        // Geometry Info
        {
            using GeoInfo = ConstraintLayout::GeoInfo;
            py::class_<GeoInfo> i2(i, "GeoInfo");

            i2.def(py::init());

            i2.def_readwrite("keepWidth", &GeoInfo::keepWidth);
            // Horz Distance
            {
                using HorzDistance = GeoInfo::HorzDistance;
                py::class_<HorzDistance> i3(i2, "HorzDistance");

                i3.def(py::init());

                i3.def_readwrite("ToLeft", &HorzDistance::ToLeft);
                i3.def_readwrite("ToRight", &HorzDistance::ToRight);
            }
            i2.def_readwrite("Left", &GeoInfo::Left);
            i2.def_readwrite("Right", &GeoInfo::Right);

            i2.def_readwrite("keepHeight", &GeoInfo::keepHeight);
            // Vert Distance
            {
                using VertDistance = GeoInfo::VertDistance;
                py::class_<VertDistance> i3(i2, "VertDistance");

                i3.def(py::init());

                i3.def_readwrite("ToTop", &VertDistance::ToTop);
                i3.def_readwrite("ToBottom", &VertDistance::ToBottom);
            }
            i2.def_readwrite("Top", &GeoInfo::Top);
            i2.def_readwrite("Bottom", &GeoInfo::Bottom);
        }
    }
}
