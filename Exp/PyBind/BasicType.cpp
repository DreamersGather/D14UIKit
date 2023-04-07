#include "Common/Precompile.h"

#include "BasicType.h"

#include "Inc/BasicType.h"

namespace d14uikit
{
    void Py_InitBasicType(py::module_& m)
    {
        // Range
        {
            py::class_<Range> i(m, "Range");

            i.def(py::init<int, int>(),
                  "offset"_a = 0, "count"_a = 0);

            i.def_readwrite("offset", &Range::offset);
            i.def_readwrite("count", &Range::count);
        }
        // Point
        {
            py::class_<Point> i(m, "Point");

            i.def(py::init<int, int>(),
                  "x"_a = 0, "y"_a = 0);

            i.def_readwrite("x", &Point::x);
            i.def_readwrite("y", &Point::y);
        }
        // Size
        {
            py::class_<Size> i(m, "Size");

            i.def(py::init<int, int>(),
                  "width"_a = 0, "height"_a = 0);

            i.def_readwrite("width", &Size::width);
            i.def_readwrite("height", &Size::height);
        }
        // Rect
        {
            py::class_<Rect> i(m, "Rect");

            i.def(py::init<int, int, int, int>(),
                  "left"_a = 0,  "top"_a = 0,
                  "right"_a = 0, "bottom"_a = 0);

            i.def_readwrite("left", &Rect::left);
            i.def_readwrite("top",  &Rect::top);
            i.def_readwrite("right",  &Rect::right);       
            i.def_readwrite("bottom", &Rect::bottom);
        }
        // Color
        {
            py::class_<Color> i(m, "Color");

            i.def(py::init<int, int, int>(),
                  "r"_a = 0, "g"_a = 0, "b"_a = 0);

            i.def_readwrite("r", &Color::r);
            i.def_readwrite("g", &Color::g);
            i.def_readwrite("b", &Color::b);
        }
        // Pixel
        {
            py::class_<Pixel> i(m, "Pixel");

            i.def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>(),
                  "b"_a = 0, "g"_a = 0, "r"_a = 0, "a"_a = 0);

            i.def_readwrite("b", &Pixel::b);
            i.def_readwrite("g", &Pixel::g);
            i.def_readwrite("r", &Pixel::r);
            i.def_readwrite("a", &Pixel::a);
        }
    }
}
