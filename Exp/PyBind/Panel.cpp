﻿#include "Common/Precompile.h"

#include "Panel.h"
#include "Callback.h"

#include "Inc/Image.h"

namespace d14uikit
{
    void Py_InitPanel(py::module_& m)
    {
        py::class_<Panel, PyPanel<>> i(m, "Panel");

        i.def(py::init<>());

        i.def(
            "destroy",
            &Panel::destory);

        i.def_property(
            "visible",
            &Panel::visible,
            &Panel::setVisible);

        i.def_property(
            "enabled",
            &Panel::enabled,
            &Panel::setEnabled);

        i.def_property(
            "size",
            &Panel::size,
            &Panel::setSize);

        i.def_property(
            "width",
            &Panel::width,
            &Panel::setWidth);

        i.def_property(
            "height",
            &Panel::height,
            &Panel::setHeight);

        i.def_property(
            "position",
            &Panel::position,
            &Panel::setPosition);

        i.def_property(
            "x",
            &Panel::x,
            &Panel::setX);

        i.def_property(
            "y",
            &Panel::y,
            &Panel::setY);

        i.def_property(
            "color",
            &Panel::color,
            &Panel::setColor);

        i.def_property(
            "opacity",
            &Panel::opacity,
            &Panel::setOpacity);

        i.def_property(
            "image",
            &Panel::image,
            &Panel::setImage);

        i.def_property(
            "roundRadius",
            &Panel::roundRadius,
            &Panel::setRoundRadius);

        i.def(
            "setGlobal",
            &Panel::setGlobal,
            "value"_a);

        i.def_property(
            "parent",
            &Panel::parent,
            &Panel::setParent);

        i.def(
            "moveTopmost",
            &Panel::moveTopmost);

        i.def(
            "mostAbove",
            &Panel::moveAbove,
            "uiobj"_a);

        i.def(
            "moveBelow",
            &Panel::moveBelow,
            "uiobj"_a);

#define DEF_PROPERTY(Name) \
_D14_CALLBACK_PROPERTY(Panel, Name)

        DEF_PROPERTY(onSize);
        DEF_PROPERTY(onMove);
        DEF_PROPERTY(onChangeTheme);
        DEF_PROPERTY(onChangeLangLocale);
        DEF_PROPERTY(onGetFocus);
        DEF_PROPERTY(onLoseFocus);
        DEF_PROPERTY(onKeyboard);
        DEF_PROPERTY(onMouseEnter);
        DEF_PROPERTY(onMouseMove);
        DEF_PROPERTY(onMouseLeave);
        DEF_PROPERTY(onMouseWheel);
        DEF_PROPERTY(onMouseButton);
        
#undef DEF_PROPERTY

        i.def(
            "onSize",
            &ExPanel::onSize);

        i.def(
            "onMove",
            &ExPanel::onMove);

        i.def(
            "onChangeTheme",
            &ExPanel::onChangeTheme);

        i.def(
            "onChangeLangLocale",
            &ExPanel::onChangeLangLocale);

        i.def(
            "onGetFocus",
            &ExPanel::onGetFocus);

        i.def(
            "onLoseFocus",
            &ExPanel::onLoseFocus);

        i.def(
            "onKeyboard",
            &ExPanel::onKeyboard);

        i.def(
            "onMouseEnter",
            &ExPanel::onMouseEnter);

        i.def(
            "onMouseMove",
            &ExPanel::onMouseMove);

        i.def(
            "onMouseLeave",
            &ExPanel::onMouseLeave);

        i.def(
            "onMouseWheel",
            &ExPanel::onMouseWheel);

        i.def(
            "onMouseButton",
            &ExPanel::onMouseButton);  
    }
}