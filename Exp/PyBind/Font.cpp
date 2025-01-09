#include "Common/Precompile.h"

#include "Font.h"

#include "EnumBind.h"

#include "Inc/Font.h"

namespace d14uikit
{
    void Py_InitFont(py::module_& m)
    {
        py::class_<Font> i(m, "Font");

        i.def(py::init<const std::wstring&>(), "name"_a);

        i.def_property_readonly("familyName", &Font::familyName);
        i.def_property_readonly("size", &Font::size);
        i.def_property_readonly("localeName", &Font::localeName);

        py::enum_<Font::Weight>(i, "Weight")
            _D14_UIKIT_PYBIND_ENUM(Font, Thin)
            _D14_UIKIT_PYBIND_ENUM(Font, ExtraLight)
            _D14_UIKIT_PYBIND_ENUM(Font, Light)
            _D14_UIKIT_PYBIND_ENUM(Font, SemiLight)
            _D14_UIKIT_PYBIND_ENUM(Font, Regular)
            _D14_UIKIT_PYBIND_ENUM(Font, SemiBold)
            _D14_UIKIT_PYBIND_ENUM(Font, Bold)
            _D14_UIKIT_PYBIND_ENUM(Font, ExtraBold)
            _D14_UIKIT_PYBIND_ENUM(Font, Black)
            _D14_UIKIT_PYBIND_ENUM(Font, ExtraBlack)
            .export_values();

        i.def_property_readonly("weight", &Font::weight);

        py::enum_<Font::Style>(i, "Style")
            _D14_UIKIT_PYBIND_ENUM(Font, Normal)
            _D14_UIKIT_PYBIND_ENUM(Font, Oblique)
            _D14_UIKIT_PYBIND_ENUM(Font, Italic)
            .export_values();

        i.def_property_readonly("style", &Font::style);

        py::enum_<Font::Stretch>(i, "Stretch")
            _D14_UIKIT_PYBIND_ENUM(Font, ExtraCondensed)
            _D14_UIKIT_PYBIND_ENUM(Font, Condensed)
            _D14_UIKIT_PYBIND_ENUM(Font, SemiCondensed)
            _D14_UIKIT_PYBIND_ENUM(Font, Medium)
            _D14_UIKIT_PYBIND_ENUM(Font, SemiExpanded)
            _D14_UIKIT_PYBIND_ENUM(Font, Expanded)
            _D14_UIKIT_PYBIND_ENUM(Font, ExtraExpanded)
            .export_values();

        i.def_property_readonly("stretch", &Font::stretch);

        i.def_static("querySystemFontNames", &Font::querySystemFontNames);

        i.def_static(
            "load",
            &Font::load,
            "name"_a,
            "familyName"_a,
            "size"_a,
            "localeName"_a,
            "weight"_a = Font::Regular,
            "style"_a = Font::Normal,
            "stretch"_a = Font::Medium);
    }
}
