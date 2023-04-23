#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit::platform_utils
{
    float dpi();

    SIZE scaledByDpi(const SIZE& sz);
    SIZE restoredByDpi(const SIZE& sz);

    POINT scaledByDpi(const POINT& pt);
    POINT restoredByDpi(const POINT& pt);

    RECT scaledByDpi(const RECT& rc);
    RECT restoredByDpi(const RECT& rc);
}
