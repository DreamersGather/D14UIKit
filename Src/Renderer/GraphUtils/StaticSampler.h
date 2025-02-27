#pragma once

#include "Common/Precompile.h"

namespace d14engine::renderer::graph_utils
{
    namespace static_sampler
    {
        ///////////
        // Point //
        ///////////

        const D3D12_STATIC_SAMPLER_DESC* pointWrap(UINT shaderRegister);

        const D3D12_STATIC_SAMPLER_DESC* pointMirror(UINT shaderRegister);

        const D3D12_STATIC_SAMPLER_DESC* pointClamp(UINT shaderRegister);

        const D3D12_STATIC_SAMPLER_DESC* pointBorder(UINT shaderRegister);

        const D3D12_STATIC_SAMPLER_DESC* pointMirrorOnce(UINT shaderRegister);

        ////////////
        // Linear //
        ////////////

        const D3D12_STATIC_SAMPLER_DESC* linearWrap(UINT shaderRegister);

        const D3D12_STATIC_SAMPLER_DESC* linearMirror(UINT shaderRegister);

        const D3D12_STATIC_SAMPLER_DESC* linearClamp(UINT shaderRegister);

        const D3D12_STATIC_SAMPLER_DESC* linearBorder(UINT shaderRegister);

        const D3D12_STATIC_SAMPLER_DESC* linearMirrorOnce(UINT shaderRegister);

        /////////////////
        // Anisotropic //
        /////////////////

        const D3D12_STATIC_SAMPLER_DESC* anisotropicWrap(UINT shaderRegister, UINT maxAnisotropy = 16U);

        const D3D12_STATIC_SAMPLER_DESC* anisotropicMirror(UINT shaderRegister, UINT maxAnisotropy = 16U);

        const D3D12_STATIC_SAMPLER_DESC* anisotropicClamp(UINT shaderRegister, UINT maxAnisotropy = 16U);

        const D3D12_STATIC_SAMPLER_DESC* anisotropicBorder(UINT shaderRegister, UINT maxAnisotropy = 16U);

        const D3D12_STATIC_SAMPLER_DESC* anisotropicMirrorOnce(UINT shaderRegister, UINT maxAnisotropy = 16U);
    };
}
