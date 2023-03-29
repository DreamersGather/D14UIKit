#pragma once

#include "Common.h"

#include "BasicType.h"

namespace d14uikit
{
    class DllExport Image : public NonCopyable
    {
        _D14_UIKIT_PIMPL_DEF

        Image(int width, int height, bool cpuRead = false);
        explicit Image(const std::wstring& path, bool cpuRead = false);

        bool cpuRead() const;

        Size size() const;
        void setSize(const Size& value);

        int width() const;
        void setWidth(int value);

        int height() const;
        void setHeight(int value);

        void load(const std::wstring& path, bool cpuRead = false);

        void copy(const Rect& dst, const Pixel* source);
        void copy(const Point& dst, Image* source, const Rect& src);

        Pixel* map(); void unmap(); // Set cpu-read before using!

    protected:
        void initialize();
        explicit Image(Passkey);
    };
}
