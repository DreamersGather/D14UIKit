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
}

/////////////////////
// Throw If Failed //
/////////////////////

#if _D14_RUNTIME_ERROR
#define THROW_IF_FAILED(Expression) \
do { \
    HRESULT throw_if_failed_hr = (Expression); \
    if (FAILED(throw_if_failed_hr)) \
    { \
        throw DirectXError(throw_if_failed_hr, __FILEW__, __LINE__); \
    } \
} while (0)
#else
#define THROW_IF_FAILED(Expression) (Expression)
#endif

////////////////////
// Throw If Error //
////////////////////

#if _D14_RUNTIME_ERROR
#define THROW_IF_ERROR(Expression) \
do { \
    ComPtr<ID3DBlob> pErrorBlob; \
    auto ppErrorBlob = &pErrorBlob; \
    HRESULT throw_if_error_hr = (Expression); \
    if (pErrorBlob) \
    { \
        OutputDebugStringA((char*)pErrorBlob->GetBufferPointer()); \
    } \
    THROW_IF_FAILED(throw_if_error_hr); \
} while (0)
#else
#define THROW_IF_ERROR(Expression) \
do { \
    constexpr auto ppErrorBlob = nullptr; (Expression); \
} while (0)
#endif
