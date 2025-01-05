#include "Common/Precompile.h"

#include "BasicEnum.h"

#include "Inc/BasicEnum.h"

namespace d14uikit
{
    void Py_InitBasicEnum(py::module_& m)
    {
        py::enum_<TextAntialiasMode>(m, "TextAntialiasMode")
            .value("Default",   TextAntialiasMode::Default)
            .value("ClearType", TextAntialiasMode::ClearType)
            .value("Grayscale", TextAntialiasMode::Grayscale)
            .value("Aliased",   TextAntialiasMode::Aliased)
            .export_values();

        py::enum_<D2DRenderingMode>(m, "D2DRenderingMode")
            .value("Default",           D2DRenderingMode::Default)
            .value("Aliased",           D2DRenderingMode::Aliased)
            .value("GDIClassic",        D2DRenderingMode::GDIClassic)
            .value("GDINatural",        D2DRenderingMode::GDINatural)
            .value("Natural",           D2DRenderingMode::Natural)
            .value("NaturalSymmetric",  D2DRenderingMode::NaturalSymmetric)
            .value("Outline",           D2DRenderingMode::Outline)
            .export_values();

        py::enum_<BitmapInterpMode>(m, "BitmapInterpMode")
            .value("NearestNeighbor",   BitmapInterpMode::NearestNeighbor)
            .value("Linear",            BitmapInterpMode::Linear)
            .value("Cubic",             BitmapInterpMode::Cubic)
            .value("MultiSampleLinear", BitmapInterpMode::MultiSampleLinear)
            .value("Anisotropic",       BitmapInterpMode::Anisotropic)
            .value("HighQualityCubic",  BitmapInterpMode::HighQualityCubic)
            .export_values();
    }
}
