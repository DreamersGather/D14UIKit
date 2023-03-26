#include "Common/Precompile.h"

#include "Image.h"

#include "Inc/Image.h"

namespace d14uikit
{
    void Py_InitImage(py::module_& m)
    {
        py::class_<Image> i(m, "Image");

        i.def(
            py::init<int, int>(),
            "width"_a,
            "height"_a);

        i.def(
            py::init<const std::wstring&>(),
            "path"_a);

        i.def_property(
            "size",
            &Image::size,
            &Image::setSize);

        i.def_property(
            "width",
            &Image::width,
            &Image::setWidth);

        i.def_property(
            "height",
            &Image::height,
            &Image::setHeight);

        i.def(
            "load",
            &Image::load,
            "path"_a);

        i.def(
            "copy",
            &Image::copy,
            "offset"_a,
            "source"_a,
            "srcArea"_a);
    }
}
