#include "Common/Precompile.h"

#include "Image.h"

#include "Inc/Image.h"

namespace d14uikit
{
    void Py_InitImage(py::module_& m)
    {
        py::class_<Image> i(m, "Image");

        i.def(py::init<int, int, bool>(),
              "width"_a, "height"_a,
              "cpuRead"_a = false);

        i.def(py::init<const std::wstring&, bool>(),
              "path"_a,
              "cpuRead"_a = false);

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
            "path"_a,
            "cpuRead"_a = false);

        i.def("copy", [](Image& self, const Rect& dst, const std::vector<Pixel>& source)
        {
            self.copy(dst, source.data());
        },
        "dst"_a, "source"_a);

        i.def("copy", py::overload_cast<const Point&, Image*, const Rect&>(&Image::copy),
        "dst"_a, "source"_a, "src"_a);

        i.def("data", [](Image& self) -> std::vector<Pixel> // Returns copied pixel 2d-array.
        {
            if (self.cpuRead())
            {
                auto srcPix = self.map();
                auto imgGeo = self.size();
                auto pixNum = imgGeo.width * imgGeo.height;
                std::vector<Pixel> dstPix(pixNum);
                memcpy(dstPix.data(), srcPix, pixNum * sizeof(Pixel));
                self.unmap();
                return dstPix;
            }
            else return {}; // empty pixel 2d-array
        });
    }
}
