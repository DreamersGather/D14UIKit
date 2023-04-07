#include "Common/Precompile.h"

#include "LabelArea.h"

#include "Panel.h"

#include "Inc/LabelArea.h"

namespace d14uikit
{
    void Py_InitLabelArea(py::module_& m)
    {
        py::class_<LabelArea, Label, PyPanel<LabelArea>> i(m, "LabelArea");

        i.def(py::init<const std::wstring&>(), "text"_a = L"");

        i.def_property("caretOffset", &LabelArea::caretOffset, &LabelArea::setCaretOffset);

        i.def_property("hiliteRange", &LabelArea::hiliteRange, &LabelArea::setHiliteRange);

        i.def("performCommandCtrlA", &LabelArea::performCommandCtrlA);

        i.def("performCommandCtrlC", &LabelArea::performCommandCtrlC);
    }
}
