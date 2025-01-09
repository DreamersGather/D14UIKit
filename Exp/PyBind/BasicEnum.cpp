#include "Common/Precompile.h"

#include "BasicEnum.h"
#include "EnumBind.h"

#include "Inc/BasicEnum.h"

namespace d14uikit
{
    void Py_InitBasicEnum(py::module_& m)
    {
        py::enum_<BitmapInterpMode>(m, "BitmapInterpMode")
            _D14_UIKIT_PYBIND_ENUM(BitmapInterpMode, NearestNeighbor)
            _D14_UIKIT_PYBIND_ENUM(BitmapInterpMode, Linear)
            _D14_UIKIT_PYBIND_ENUM(BitmapInterpMode, Cubic)
            _D14_UIKIT_PYBIND_ENUM(BitmapInterpMode, MultiSampleLinear)
            _D14_UIKIT_PYBIND_ENUM(BitmapInterpMode, Anisotropic)
            _D14_UIKIT_PYBIND_ENUM(BitmapInterpMode, HighQualityCubic)
            .export_values();

        py::enum_<TextAntialiasMode>(m, "TextAntialiasMode")
            _D14_UIKIT_PYBIND_ENUM(TextAntialiasMode, Default)
            _D14_UIKIT_PYBIND_ENUM(TextAntialiasMode, ClearType)
            _D14_UIKIT_PYBIND_ENUM(TextAntialiasMode, Grayscale)
            _D14_UIKIT_PYBIND_ENUM(TextAntialiasMode, Aliased)
            .export_values();

        py::enum_<RenderingMode>(m, "RenderingMode")
            _D14_UIKIT_PYBIND_ENUM(RenderingMode, Default)
            _D14_UIKIT_PYBIND_ENUM(RenderingMode, Aliased)
            _D14_UIKIT_PYBIND_ENUM(RenderingMode, GDIClassic)
            _D14_UIKIT_PYBIND_ENUM(RenderingMode, GDINatural)
            _D14_UIKIT_PYBIND_ENUM(RenderingMode, Natural)
            _D14_UIKIT_PYBIND_ENUM(RenderingMode, NaturalSymmetric)
            _D14_UIKIT_PYBIND_ENUM(RenderingMode, Outline)
            .export_values();

        py::enum_<DrawTextOption>(m, "DrawTextOption")
            _D14_UIKIT_PYBIND_ENUM(DrawTextOption, None)
            _D14_UIKIT_PYBIND_ENUM(DrawTextOption, Clip)
            .export_values();
    }
}
