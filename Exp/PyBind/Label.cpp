#include "Common/Precompile.h"

#include "Label.h"
#include "Panel.h"

#include "Inc/Label.h"

namespace d14uikit
{
    void Py_InitLabel(py::module_& m)
    {
        py::class_<Label, Panel, PyPanel<Label>> i(m, "Label");

        i.def(py::init<const std::wstring&>(), "text"_a = L"");

        i.def_property("text", &Label::text, &Label::setText);

        i.def_property("font", &Label::font, &Label::setFont);

        py::enum_<Label::HorzAlign>(i, "HorzAlign")
            .value("Left",      Label::Left)
            .value("HCenter",   Label::HCenter)
            .value("Right",     Label::Right)
            .value("Justified", Label::Justified)
            .export_values();

        py::enum_<Label::VertAlign>(i, "VertAlign")
            .value("Top",       Label::Top)
            .value("VCenter",   Label::VCenter)
            .value("Bottom",    Label::Bottom)
            .export_values();

        i.def_property(
            "horzAlign",
            &Label::horzAlign,
            &Label::setHorzAlign);

        i.def_property(
            "horzHardAlign",
            &Label::horzHardAlign,
            &Label::setHorzHardAlign);

        i.def_property(
            "vertAlign",
            &Label::vertAlign,
            &Label::setVertAlign);

        i.def_property(
            "vertHardAlign",
            &Label::vertHardAlign,
            &Label::setVertHardAlign);

        py::enum_<Label::WordWrapping>(i, "WordWrapping")
            .value("Character",         Label::Character)
            .value("NoWrap",            Label::NoWrap)
            .value("WholeWord",         Label::WholeWord)
            .value("Wrap",              Label::Wrap)
            .value("EmergencyBreak",    Label::EmergencyBreak)
            .export_values();

        i.def_property(
            "wordWrapping",
            &Label::wordWrapping,
            &Label::setWordWrapping);

        i.def_property(
            "drawTextClip",
            &Label::drawTextClip,
            &Label::setDrawTextClip);
    }
}
