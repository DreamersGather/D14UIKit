#include "Common/Precompile.h"

#include "Application.h"

#include "Inc/Application.h"
#include "Inc/Cursor.h"

namespace d14uikit
{
    void Py_InitApplication(py::module_& m)
    {
        py::class_<Application> i(m, "Application");

        i.def(
            py::init<const std::wstring&>(),
            "name"_a = L"D14UIKit");

        i.def_property_readonly_static(
            "app",
            [](py::object /* class.type */)
            {
                return Application::app();
            });

        i.def_property_readonly(
            "cursor",
            &Application::cursor);

        i.def(
            "run",
            &Application::run);

        i.def(
            "exit",
            &Application::exit);

        i.def_property(
            "size",
            &Application::size,
            &Application::setSize);

        i.def_property(
            "width",
            &Application::width,
            &Application::setWidth);

        i.def_property(
            "height",
            &Application::height,
            &Application::setHeight);

        i.def_property(
            "position",
            &Application::position,
            &Application::setPosition);

        i.def_property(
            "x",
            &Application::x,
            &Application::setX);

        i.def_property(
            "y",
            &Application::y,
            &Application::setY);

        i.def_property(
            "minSize",
            &Application::minSize,
            &Application::setMinSize);

        i.def_property(
            "minWidth",
            &Application::minWidth,
            &Application::setMinWidth);

        i.def_property(
            "minHeight",
            &Application::minHeight,
            &Application::setMinHeight);

        i.def_property(
            "resizable",
            &Application::resizable,
            &Application::setResizable);

        i.def_property(
            "fullscreen",
            &Application::fullscreen,
            &Application::setFullscreen);

        i.def_property_readonly(
            "fps",
            &Application::fps);

        i.def_property(
            "lowEnergy",
            &Application::lowEnergy,
            &Application::setLowEnergy);

        i.def_property(
            "themeMode",
            &Application::themeMode,
            &Application::setThemeMode);

        i.def_property(
            "themeColor",
            &Application::themeColor,
            &Application::setThemeColor);

        i.def_property(
            "useSystemTheme",
            &Application::useSystemTheme,
            &Application::setUseSystemTheme);

        i.def_property(
            "langLocale",
            &Application::langLocale,
            &Application::setLangLocale);

        i.def_property(
            "clearType",
            &Application::clearType,
            &Application::setClearType);

        i.def_property(
            "drawTextNatrualSymmetric",
            &Application::drawTextNatrualSymmetric,
            &Application::setDrawTextNatrualSymmetric);
    }
}
