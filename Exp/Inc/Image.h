#pragma once

#include "Common.h"

#include "BasicType.h"

namespace d14uikit
{
    class DllExport Image : public NonCopyable
    {
        _D14_UIKIT_PIMPL_DEF

        Image(int width, int height, bool cpuRead = true);
        explicit Image(const std::wstring& path, bool cpuRead = false);

        bool cpuRead() const;

        Size size() const;
        void setSize(const Size& value);

        int width() const;
        void setWidth(int value);

        int height() const;
        void setHeight(int value);

        void load(const std::wstring& path, bool cpuRead = false);

        bool copy(Rect dst, Pixel* source);
        bool copy(Point dst, Image* source, Rect src);

        Pixel* map(); bool unmap(); // Idle the GPU before using!

    protected:
        void initialize();
        explicit Image(Passkey);
    };
}
