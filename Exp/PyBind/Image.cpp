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

        i.def_property_readonly(
            "dipSize",
            &Image::dipSize);

        i.def_property_readonly(
            "dipWidth",
            &Image::dipWidth);

        i.def_property_readonly(
            "dipHeight",
            &Image::dipHeight);

        py::enum_<Image::Format>(i, "Format")
            .value("Adng",  Image::Adng)
            .value("Bmp",   Image::Bmp)
            .value("Png",   Image::Png)
            .value("Ico",   Image::Ico)
            .value("Jpeg",  Image::Jpeg)
            .value("Tiff",  Image::Tiff)
            .value("Gif",   Image::Gif)
            .value("Wmp",   Image::Wmp)
            .value("Heif",  Image::Heif)
            .value("Webp",  Image::Webp)
            .export_values();

        i.def(
            "load",
            &Image::load,
            "path"_a,
            "cpuRead"_a = false);

        i.def(
            "save",
            &Image::save,
            "path"_a,
            "format"_a = Image::Png);

#define COPY_LAMBDA (Image& self, const Rect& dst, const std::vector<Pixel>& source)
#define COPY_OVERLOAD(Name) py::overload_cast<const Point&, Image*, const Rect&>(Name)

        i.def("copy", []COPY_LAMBDA
        {
            self.copy(dst, source.data());
        },
        "dst"_a, "source"_a);

        i.def("copy", COPY_OVERLOAD(&Image::copy),
        "dst"_a, "source"_a, "src"_a);

        i.def("copyInFrame", []COPY_LAMBDA
        {
            self.copy(dst, source.data());
        },
        "dst"_a, "source"_a);

        i.def("copyInFrame", COPY_OVERLOAD(&Image::copyInFrame),
        "dst"_a, "source"_a, "src"_a);

#undef COPY_LAMBDA
#undef COPY_OVERLOAD

        i.def("data", [](Image& self) -> std::vector<Pixel>
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
