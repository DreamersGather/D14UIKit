#pragma once

#include "Common.h"

namespace d14uikit
{
    class DllExport MainWindow
    {
    public:
        MainWindow(
            const std::wstring& title = L"Untitled",
            float captionPanelHeight = 32.0f,
            float decorativeBarHeight = 4.0f);

        ~MainWindow();

    private:
        struct Impl;
        std::unique_ptr<Impl> pimpl = {};

    public:

    };
}
