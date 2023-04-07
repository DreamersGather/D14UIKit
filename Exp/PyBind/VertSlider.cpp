#include "Common/Precompile.h"

#include "VertSlider.h"

#include "Slider.h"

namespace d14uikit
{
    void Py_InitVertSlider(py::module_& m)
    {
        py::class_<VertSlider, Panel, PyVertSlider<>> i(m, "VertSlider");

        i.def(py::init());

        _D14_UIKIT_PYBIND_SLIDER(VertSlider)
    }
}
