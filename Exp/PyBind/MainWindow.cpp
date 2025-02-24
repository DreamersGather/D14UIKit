#include "Common/Precompile.h"

#include "MainWindow.h"

#include "Window.h"

#include "Inc/MainWindow.h"

namespace d14uikit
{
    void Py_InitMainWindow(py::module_& m)
    {
        py::class_<MainWindow, Window, PyWindow<MainWindow>> i(m, "MainWindow");

        i.def(py::init<const std::wstring&>(), "title"_a = L"Untitled");
    }
}
