#include "Common/Precompile.h"

#include "GridLayout.h"

#include "ResizablePanel.h"

#include "Inc/GridLayout.h"

namespace d14uikit
{
    void Py_InitGridLayout(py::module_& m)
    {
        py::class_<GridLayout, ResizablePanel,
            PyResizablePanel<GridLayout>> i(m, "GridLayout");

        i.def(py::init());

        i.def_property(
            "horzSpacing",
            &GridLayout::horzSpacing,
            &GridLayout::setHorzSpacing);

        i.def_property(
            "vertSpacing",
            &GridLayout::vertSpacing,
            &GridLayout::setVertCellCount);

        i.def_property(
            "horzCellCount",
            &GridLayout::horzCellCount,
            &GridLayout::setHorzCellCount);

        i.def_property(
            "vertCellCount",
            &GridLayout::vertCellCount,
            &GridLayout::setVertCellCount);

        // Geometry Info
        {
            using GeoInfo = GridLayout::GeoInfo;
            py::class_<GeoInfo> i2(i, "GeoInfo");

            i2.def(py::init());

            i2.def_readwrite("fixedSize", &GeoInfo::fixedSize);
            // Axis
            {
                using Axis = GeoInfo::Axis;
                py::class_<Axis> i3(i2, "Axis");

                i3.def(py::init());

                i3.def_readwrite("offset", &Axis::offset);
                i3.def_readwrite("count", &Axis::count);
            }
            i2.def_readwrite("x", &GeoInfo::x);
            i2.def_readwrite("y", &GeoInfo::y);

            i2.def_readwrite("spacing", &GeoInfo::spacing);
        }

        i.def("addElement", &GridLayout::addElement);

        i.def("removeElement", &GridLayout::removeElement);
        i.def("clearAllElements", &GridLayout::clearAllElements);

        i.def("updateElement", &GridLayout::updateElement);
    }
}
