﻿#pragma once

#include "Common.h"

#include "BasicType.h"

namespace d14uikit
{
    class DllExport Image : public _D14_UIKIT_NONCOPYABLE
    {
        _D14_UIKIT_PIMPL_DEF

        Image();
        Image(int width, int height);
        Image(const std::wstring& path);

        Size size() const;
        void setSize(const Size& value);

        int width() const;
        void setWidth(int value);

        int height() const;
        void setHeight(int value);

        void load(const std::wstring& path);
        void copy(Point offset, Image* source, Rect srcArea);

    protected:
        void initialize();
        explicit Image(Passkey);
    };
}