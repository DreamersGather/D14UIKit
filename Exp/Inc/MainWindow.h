﻿#pragma once

#include "Window.h"

namespace d14uikit
{
    class DllExport MainWindow : public Window
    {
        _D14_UIKIT_PIMPL(MainWindow)

        explicit MainWindow(const std::wstring& title = L"Untitled");

        void showNormal();
        void showMinimized();
        void showMaximized();
    };
}
