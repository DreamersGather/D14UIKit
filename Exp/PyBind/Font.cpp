#include "Common/Precompile.h"

#include "Font.h"

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
            .value("Thin",          Font::Thin)
            .value("ExtraLight",    Font::ExtraLight)
            .value("Light",         Font::Light)
            .value("SemiLight",     Font::SemiLight)
            .value("Regular",       Font::Regular)
            .value("SemiBold",      Font::SemiBold)
            .value("Bold",          Font::Bold)
            .value("ExtraBold",     Font::ExtraBold)
            .value("Black",         Font::Black)
            .value("ExtraBlack",    Font::ExtraBlack)
            .export_values();

        i.def_property_readonly("weight", &Font::weight);

        py::enum_<Font::Style>(i, "Style")
            .value("Normal",    Font::Normal)
            .value("Oblique",   Font::Oblique)
            .value("Italic",    Font::Italic)
            .export_values();

        i.def_property_readonly("style", &Font::style);

        py::enum_<Font::Stretch>(i, "Stretch")
            .value("ExtraCondensed",    Font::ExtraCondensed)
            .value("Condensed",         Font::Condensed)
            .value("SemiCondensed",     Font::SemiCondensed)
            .value("Medium",            Font::Medium)
            .value("SemiExpanded",      Font::SemiExpanded)
            .value("Expanded",          Font::Expanded)
            .value("ExtraExpanded",     Font::ExtraExpanded)
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
            "weight"_a,
            "style"_a,
            "stretch"_a);
    }
}
