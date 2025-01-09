#include "Common/Precompile.h"

#include "Label.h"

#include "EnumBind.h"
#include "Panel.h"

#include "Inc/Label.h"

namespace d14uikit
{
    void Py_InitLabel(py::module_& m)
    {
        py::class_<Label, Panel, PyPanel<Label>> i(m, "Label");

        i.def(py::init<const std::wstring&>(), "text"_a = L"");

        i.def_property(
            "bkgnColor",
            &Label::bkgnColor,
            &Label::setBkgnColor);

        i.def_property(
            "bkgnOpacity",
            &Label::bkgnOpacity,
            &Label::setBkgnOpacity);

        i.def_property(
            "frgnColor",
            &Label::frgnColor,
            &Label::setFrgnColor);

        i.def_property(
            "frgnOpacity",
            &Label::frgnOpacity,
            &Label::setFrgnOpacity);

        i.def_property(
            "text",
            &Label::text,
            &Label::setText);

        i.def_property(
            "drawTextOption",
            &Label::drawTextOption,
            &Label::setDrawTextOption);

        i.def_property(
            "font",
            &Label::font,
            &Label::setFont);

        py::enum_<Label::HorzAlign>(i, "HorzAlign")
            _D14_UIKIT_PYBIND_ENUM(Label, Left)
            _D14_UIKIT_PYBIND_ENUM(Label, Right)
            _D14_UIKIT_PYBIND_ENUM(Label, HCenter)
            _D14_UIKIT_PYBIND_ENUM(Label, Justified)
            .export_values();

        py::enum_<Label::VertAlign>(i, "VertAlign")
            _D14_UIKIT_PYBIND_ENUM(Label, Top)
            _D14_UIKIT_PYBIND_ENUM(Label, Bottom)
            _D14_UIKIT_PYBIND_ENUM(Label, VCenter)
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
            _D14_UIKIT_PYBIND_ENUM(Label, Wrap)
            _D14_UIKIT_PYBIND_ENUM(Label, NoWrap)
            _D14_UIKIT_PYBIND_ENUM(Label, EmergencyBreak)
            _D14_UIKIT_PYBIND_ENUM(Label, WholeWord)
            _D14_UIKIT_PYBIND_ENUM(Label, Character)
            .export_values();

        i.def_property(
            "wordWrapping",
            &Label::wordWrapping,
            &Label::setWordWrapping);

        i.def_property(
            "fontFamilyName",
            &Label::fontFamilyName,
            [](Label& self, const std::wstring& name)
            {
                self.setFontFamilyName(name);
            });
        i.def(
            "setFontFamilyName",
            &Label::setFontFamilyName,
            "name"_a,
            "range"_a = std::nullopt);

        i.def_property(
            "fontSize",
            &Label::fontSize,
            [](Label& self, int value)
            {
                self.setFontSize(value);
            });
        i.def(
            "setFontSize",
            &Label::setFontSize,
            "value"_a,
            "range"_a = std::nullopt);

        i.def_property(
            "fontLocaleName",
            &Label::fontLocaleName,
            [](Label& self, const std::wstring& name)
            {
                self.setFontLocaleName(name);
            });
        i.def(
            "setFontLocaleName",
            &Label::setFontLocaleName,
            "name"_a,
            "range"_a = std::nullopt);

        i.def_property(
            "fontWeight",
            &Label::fontWeight,
            [](Label& self, Font::Weight value)
            {
                self.setFontWeight(value);
            });
        i.def(
            "setFontWeight",
            &Label::setFontWeight,
            "value"_a,
            "range"_a = std::nullopt);

        i.def_property(
            "fontStyle",
            &Label::fontStyle,
            [](Label& self, Font::Style value)
            {
                self.setFontStyle(value);
            });
        i.def(
            "setFontStyle",
            &Label::setFontStyle,
            "value"_a,
            "range"_a = std::nullopt);

        i.def_property(
            "fontStretch",
            &Label::fontStretch,
            [](Label& self, Font::Stretch value)
            {
                self.setFontStretch(value);
            });
        i.def(
            "setFontStretch",
            &Label::setFontStretch,
            "value"_a,
            "range"_a = std::nullopt);

        i.def_property(
            "underline",
            [](Label& self)
            {
                return self.underline(0);
            },
            [](Label& self, bool value)
            {
                self.setUnderline(value);
            });
        i.def(
            "getUnderline",
            &Label::underline,
            "offset"_a);
        i.def(
            "setUnderline",
            &Label::setUnderline,
            "value"_a,
            "range"_a = std::nullopt);

        i.def_property(
            "strikethrough",
            [](Label& self)
            {
                return self.strikethrough(0);
            },
            [](Label& self, bool value)
            {
                self.setStrikethrough(value);
            });
        i.def(
            "getStrikethrough",
            &Label::strikethrough,
            "offset"_a);
        i.def(
            "setStrikethrough",
            &Label::setStrikethrough,
            "value"_a,
            "range"_a = std::nullopt);
    }
}
