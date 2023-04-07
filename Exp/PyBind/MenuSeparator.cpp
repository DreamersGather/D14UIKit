#include "Common/Precompile.h"

#include "MenuSeparator.h"

#include "Panel.h"

#include "Inc/MenuSeparator.h"

namespace d14uikit
{
    void Py_InitMenuSeparator(py::module_& m)
    {
        py::class_<MenuSeparator, MenuItem, PyPanel<MenuSeparator>> i(m, "MenuSeparator");

        i.def(py::init());
    }
}
