#include "Common/Precompile.h"

#ifdef _D14_UIKIT_EXP_PY_BIND

#include "Common.h"

#include "Inc/BasicType.h"

using namespace d14uikit;

void initBasicType(py::module_& m)
{
    // Point
    {
        py::class_<Point> i(m, "Point");

        i.def_readwrite("x", &Point::x);
        i.def_readwrite("y", &Point::y);
    }
    // Size
    {
        py::class_<Size> i(m, "Size");

        i.def_readwrite("width", &Size::width);
        i.def_readwrite("height", &Size::height);
    }
    // Rect
    {
        py::class_<Rect> i(m, "Rect");

        i.def_readwrite("left", &Rect::left);
        i.def_readwrite("top",  &Rect::top);
        i.def_readwrite("right",  &Rect::right);       
        i.def_readwrite("bottom", &Rect::bottom);
    }
    // Color
    {
        py::class_<Color> i(m, "Color");

        i.def_readwrite("r", &Color::r);
        i.def_readwrite("g", &Color::g);
        i.def_readwrite("b", &Color::b);
    }
}

#endif
