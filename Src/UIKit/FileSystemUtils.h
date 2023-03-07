#pragma once

#include "Common/Precompile.h"

namespace d14engine::file_system_utils
{
    Wstring extractFileName(WstrParam path);

    Wstring extractFilePrefix(WstrParam fileName, WCHAR token = L'.');

    Wstring extractFileSuffix(WstrParam fileName, WCHAR token = L'.');

    // Return true to terminate searching next file in foreachFileInDir.
    using FileOperationFunc = Function<bool(WstrParam /* file path */)>;

    // Returns whether the specified file is located successfully.
    // wildcard examples: "*" for all files, "*.png" for all PNG images, etc.
    bool foreachFileInDir(WstrParam dir, WstrParam wildcard, const FileOperationFunc& func);
}
