#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer::graph_utils
{
    namespace static_sampler
    {
        D3D12_STATIC_SAMPLER_DESC pointWrap(UINT shaderRegister);
        D3D12_STATIC_SAMPLER_DESC pointMirror(UINT shaderRegister);
        D3D12_STATIC_SAMPLER_DESC pointClamp(UINT shaderRegister);
        D3D12_STATIC_SAMPLER_DESC pointBorder(UINT shaderRegister);
        D3D12_STATIC_SAMPLER_DESC pointMirrorOnce(UINT shaderRegister);
        D3D12_STATIC_SAMPLER_DESC linearWrap(UINT shaderRegister);
        D3D12_STATIC_SAMPLER_DESC linearMirror(UINT shaderRegister);
        D3D12_STATIC_SAMPLER_DESC linearClamp(UINT shaderRegister);
        D3D12_STATIC_SAMPLER_DESC linearBorder(UINT shaderRegister);
        D3D12_STATIC_SAMPLER_DESC linearMirrorOnce(UINT shaderRegister);
        D3D12_STATIC_SAMPLER_DESC anisotropicWrap(UINT shaderRegister, UINT maxAnisotropy = 16);
        D3D12_STATIC_SAMPLER_DESC anisotropicMirror(UINT shaderRegister, UINT maxAnisotropy = 16);
        D3D12_STATIC_SAMPLER_DESC anisotropicClamp(UINT shaderRegister, UINT maxAnisotropy = 16);
        D3D12_STATIC_SAMPLER_DESC anisotropicBorder(UINT shaderRegister, UINT maxAnisotropy = 16);
        D3D12_STATIC_SAMPLER_DESC anisotropicMirrorOnce(UINT shaderRegister, UINT maxAnisotropy = 16);
    };
}
