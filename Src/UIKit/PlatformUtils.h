#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit::platform_utils
{
    float dpi(); // DPI: Dots Per Inch

    LONG scaledByDpi(LONG a);
    LONG restoredByDpi(LONG a);

    SIZE scaledByDpi(const SIZE& sz);
    SIZE restoredByDpi(const SIZE& sz);

    POINT scaledByDpi(const POINT& pt);
    POINT restoredByDpi(const POINT& pt);

    RECT scaledByDpi(const RECT& rc);
    RECT restoredByDpi(const RECT& rc);
}
