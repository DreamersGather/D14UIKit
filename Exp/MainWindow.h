#pragma once

#include "Common/Precompile.h"

#include "Inc/MainWindow.h"

namespace d14uikit
{
    struct MainWindow::Impl
    {
        std::shared_ptr<d14engine::uikit::MainWindow> uiobj = {};
    };
}
