#pragma once

// Refer to https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/#header-include-order
// It is recommended to include Agility SDK headers before the Windows 10 SDK to avoid conflicts.
#include "Renderer.h"
#include "d3dx12/d3dx12.h"

// Refer to https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/#2.-set-agility-sdk-parameters
// We need to set Agility SDK parameters to help locate the custom D3D12Core.dll during building.
extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 615; }
#ifdef _WIN64
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = "library/DirectX/x64/"; }
#else
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = "library/DirectX/x86/"; }
#endif

#include "DirectXShaderCompiler/dxcapi.h"

#pragma comment(lib, "dxcompiler.lib")
