#pragma once

#include "Common/Precompile.h"

#include "Inc/Cursor.h"

namespace d14uikit
{
    struct Cursor::Impl
    {
        std::shared_ptr<d14engine::uikit::Cursor> uiobj = {};
    };
}
