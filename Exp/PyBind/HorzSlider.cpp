#include "Common/Precompile.h"

#include "HorzSlider.h"

#include "Slider.h"

namespace d14uikit
{
    void Py_InitHorzSlider(py::module_& m)
    {
        py::class_<HorzSlider, Panel, PyHorzSlider<>> i(m, "HorzSlider");

        i.def(py::init());

        _D14_UIKIT_PYBIND_SLIDER(HorzSlider)
    }
}
