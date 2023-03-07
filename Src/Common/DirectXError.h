#pragma once

#include "Common/Precompile.h"

#include "Common/RuntimeError.h"

namespace d14engine
{
    struct DirectXError : RuntimeError
    {
        HRESULT hr = E_UNEXPECTED;

        DirectXError(HRESULT hr, WstrParam fileName, UINT lineNumber);
    };

#define THROW_IF_FAILED(Expression) \
do { \
    HRESULT throw_if_failed_hr = (Expression); \
    if (FAILED(throw_if_failed_hr)) \
    { \
        throw DirectXError(throw_if_failed_hr, __FILEW__, __LINE__); \
    } \
} while (0)

#define THROW_IF_ERROR(Expression) \
do { \
    ComPtr<ID3DBlob> error; \
    HRESULT throw_if_error_hr = (Expression); \
    if (error) \
    { \
        OutputDebugStringA((char*)error->GetBufferPointer()); \
    } \
    THROW_IF_FAILED(throw_if_error_hr); \
} while (0)

}
