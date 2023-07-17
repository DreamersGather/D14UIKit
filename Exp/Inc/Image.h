#pragma once

#include "Common.h"

#include "BasicType.h"

namespace d14uikit
{
    class DllExport Image : public NonCopyable
    {
        friend class Application;

        _D14_UIKIT_PIMPL(Image)

        Image(int width, int height, bool cpuRead = false);
        explicit Image(const std::wstring& path, bool cpuRead = false);

        bool cpuRead() const;

        Size size() const;
        void setSize(const Size& value);

        int width() const;
        void setWidth(int value);

        int height() const;
        void setHeight(int value);

        enum class Format
        {
            Adng,
            Bmp,
            Png,
            Ico,
            Jpeg,
            Tiff,
            Gif,
            Wmp,
            Heif,
            Webp
        };
#define SET_FORMAT(Name) constexpr static auto Name = Format::Name

        SET_FORMAT(Adng);
        SET_FORMAT(Bmp);
        SET_FORMAT(Png);
        SET_FORMAT(Ico);
        SET_FORMAT(Jpeg);
        SET_FORMAT(Tiff);
        SET_FORMAT(Gif);
        SET_FORMAT(Wmp);
        SET_FORMAT(Heif);
        SET_FORMAT(Webp);

#undef SET_FORMAT

        void load(const std::wstring& path, bool cpuRead = false);
        void save(const std::wstring& path, Format format = Png);

        // It is worth noting that all Rect-parameters related to
        // copy-methods are left-closed and right-open and starts
        // from 0, which means that you should pass Rect(0,0,1,1)
        // instead of Rect(0,0,0,0) to copy the left-top pixel ().

        void copy(const Rect& dst, const Pixel* source);
        void copy(const Point& dst, Image* source, const Rect& src);

        void copyInFrame(const Rect& dst, const Pixel* source);
        void copyInFrame(const Point& dst, Image* source, const Rect& src);

        Pixel* map(); void unmap(); // Set cpu-read before using!
    };
}
