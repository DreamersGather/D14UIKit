#pragma once

#include "Inc/MainWindow.h"

namespace d14engine::uikit { struct MainWindow; }

namespace d14uikit
{
    struct MainWindow::Impl
    {
        std::shared_ptr<d14engine::uikit::MainWindow> uiobj = {};
    };
}
