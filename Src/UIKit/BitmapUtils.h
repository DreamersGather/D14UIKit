#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit::bitmap_utils
{
    ComPtr<ID2D1Bitmap1> loadBitmap(UINT width, UINT height, BYTE* data = nullptr, D2D1_BITMAP_OPTIONS options = D2D1_BITMAP_OPTIONS_NONE);

    ComPtr<ID2D1Bitmap1> loadBitmap(WstrParam imageFile, WstrParam binaryPath = L"", D2D1_BITMAP_OPTIONS options = D2D1_BITMAP_OPTIONS_NONE);

    ComPtr<ID2D1Bitmap1> loadPackedBitmap(WstrParam resName, WstrParam resType = L"PNG", D2D1_BITMAP_OPTIONS options = D2D1_BITMAP_OPTIONS_NONE);

    void saveBitmap(ID2D1Bitmap1* image, WstrParam imageFile, const GUID& format = GUID_ContainerFormatPng);
}
