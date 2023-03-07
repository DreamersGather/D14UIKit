#pragma once

#include "Common.h"

#include "BasicTypes.h"

namespace d14uikit
{
    class DllExport Application
    {
    public:
        struct CreateInfo
        {
            std::wstring name = L"D14UIKit";

            bool centered = true;
            bool maximized = false;
            bool fullscreen = false;

            std::optional<Rect> rect = std::nullopt;
        };
        Application(int argc, wchar_t* argv[], const CreateInfo& info = {});

        ~Application();

    private:
        struct Impl;
        std::shared_ptr<Impl> pimpl = {};

    public:
        static Application* app();

        int run(const std::function<void(Application* app)>& onLaunch = {});

        void exit();
    };
}
