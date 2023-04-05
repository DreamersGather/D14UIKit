#pragma once

#include "Common/Precompile.h"

#ifdef _D14_UIKIT_PYBIND_DEBUG
#define PYBIND11_DETAILED_ERROR_MESSAGES
#endif

#include <pybind11/pybind11.h>

#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace d14uikit
{
    namespace py = pybind11;
    using namespace py::literals;
}
