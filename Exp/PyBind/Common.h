#pragma once

#ifdef _D14_UIKIT_EXP_PY_BIND

#ifdef _D14_UIKIT_EXP_PY_BIND_DEBUG
#define PYBIND11_DETAILED_ERROR_MESSAGES
#endif

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace d14uikit
{
    namespace py = pybind11;
    using namespace py::literals;
}

#endif
