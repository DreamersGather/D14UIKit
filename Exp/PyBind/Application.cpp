#include "Common/Precompile.h"

#ifdef _D14_UIKIT_EXP_PY_BIND

#include "Common.h"

#include "Inc/Application.h"

using namespace d14uikit;

void initApplication(py::module_& m)
{
    py::class_<Application> i(m, "Application");

    i.def(py::init([](const std::wstring& name)
    {
        return std::make_unique<Application>(0, nullptr, name);
    }
    ), py::arg("name") = L"D14UIKit");

    i.def_property_readonly_static("app", &Application::app);

    i.def("run", &Application::run);
    i.def("exit", &Application::exit);

    i.def_property("size", &Application::size, &Application::setSize);
}

#endif
