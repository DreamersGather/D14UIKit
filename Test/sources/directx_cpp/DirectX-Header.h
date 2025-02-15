#pragma once

// Refer to https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/#header-include-order
// It is recommended to include Agility SDK headers before the Windows 10 SDK to avoid conflicts.
#include "Renderer.h"

#if AGILITY_SDK
#include "d3dx12/d3dx12.h"
#endif

// Refer to https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/#2.-set-agility-sdk-parameters
// We need to set Agility SDK parameters to help locate the custom D3D12Core.dll during building.
#if AGILITY_SDK
extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = AGILITY_VER; }
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = AGILITY_PATH; }
#endif

#pragma comment(lib, "D3D12.lib")

#if DX_COMPILER
#include "DirectXShaderCompiler/dxcapi.h"
#else
#include <d3dcompiler.h>
#endif

#if DX_COMPILER
#pragma comment(lib, "dxcompiler.lib")
#else
#pragma comment(lib, "d3dcompiler.lib")
#endif

#include <stdexcept>

#define THROW_IF_FAILED(Expression) \
do { \
    HRESULT throw_if_failed_hr = (Expression); \
    if (FAILED(throw_if_failed_hr)) \
    { \
        throw std::runtime_error(#Expression); \
    } \
} while (0)
