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

#include "DirectXShaderCompiler/dxcapi.h"

#pragma comment(lib, "dxcompiler.lib")
