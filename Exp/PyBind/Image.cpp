#include "Common/Precompile.h"

#include "Image.h"

#include "Inc/Image.h"

namespace d14uikit
{
    void Py_InitImage(py::module_& m)
    {
        py::class_<Image> i(m, "Image");

        i.def(
            py::init([](const std::wstring& path)
            {
                return std::make_unique<Image>(path);
            }),
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
            [](Image& self, const std::wstring& path)
            {
                self.load(path); // no cpu-read for py
            },
            "path"_a);
    }
}
