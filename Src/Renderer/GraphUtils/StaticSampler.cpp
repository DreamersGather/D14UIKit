#include "Common/Precompile.h"

#include "Renderer/GraphUtils/StaticSampler.h"

namespace d14engine::renderer::graph_utils
{
    namespace static_sampler
    {
        D3D12_STATIC_SAMPLER_DESC pointWrap(UINT shaderRegister)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_MIN_MAG_MIP_POINT,
                D3D12_TEXTURE_ADDRESS_MODE_WRAP,
                D3D12_TEXTURE_ADDRESS_MODE_WRAP,
                D3D12_TEXTURE_ADDRESS_MODE_WRAP);
        }

        D3D12_STATIC_SAMPLER_DESC pointMirror(UINT shaderRegister)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_MIN_MAG_MIP_POINT,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR);
        }

        D3D12_STATIC_SAMPLER_DESC pointClamp(UINT shaderRegister)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_MIN_MAG_MIP_POINT,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
        }

        D3D12_STATIC_SAMPLER_DESC pointBorder(UINT shaderRegister)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_MIN_MAG_MIP_POINT,
                D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                0.0F, 16U, D3D12_COMPARISON_FUNC_LESS_EQUAL,
                D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK);
        }

        D3D12_STATIC_SAMPLER_DESC pointMirrorOnce(UINT shaderRegister)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_MIN_MAG_MIP_POINT,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE);
        }

        D3D12_STATIC_SAMPLER_DESC linearWrap(UINT shaderRegister)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_MIN_MAG_MIP_LINEAR,
                D3D12_TEXTURE_ADDRESS_MODE_WRAP,
                D3D12_TEXTURE_ADDRESS_MODE_WRAP,
                D3D12_TEXTURE_ADDRESS_MODE_WRAP);
        }

        D3D12_STATIC_SAMPLER_DESC linearMirror(UINT shaderRegister)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_MIN_MAG_MIP_LINEAR,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR);
        }

        D3D12_STATIC_SAMPLER_DESC linearClamp(UINT shaderRegister)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_MIN_MAG_MIP_LINEAR,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
        }

        D3D12_STATIC_SAMPLER_DESC linearBorder(UINT shaderRegister)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_MIN_MAG_MIP_LINEAR,
                D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                0.0F, 16U, D3D12_COMPARISON_FUNC_LESS_EQUAL,
                D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK);
        }

        D3D12_STATIC_SAMPLER_DESC linearMirrorOnce(UINT shaderRegister)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_MIN_MAG_MIP_LINEAR,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE);
        }

        D3D12_STATIC_SAMPLER_DESC anisotropicWrap(UINT shaderRegister, UINT maxAnisotropy)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_ANISOTROPIC,
                D3D12_TEXTURE_ADDRESS_MODE_WRAP,
                D3D12_TEXTURE_ADDRESS_MODE_WRAP,
                D3D12_TEXTURE_ADDRESS_MODE_WRAP,
                0.0f, maxAnisotropy);
        }

        D3D12_STATIC_SAMPLER_DESC anisotropicMirror(UINT shaderRegister, UINT maxAnisotropy)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_ANISOTROPIC,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
                0.0f, maxAnisotropy);
        }

        D3D12_STATIC_SAMPLER_DESC anisotropicClamp(UINT shaderRegister, UINT maxAnisotropy)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_ANISOTROPIC,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
                0.0f, maxAnisotropy);
        }

        D3D12_STATIC_SAMPLER_DESC anisotropicBorder(UINT shaderRegister, UINT maxAnisotropy)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_ANISOTROPIC,
                D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                D3D12_TEXTURE_ADDRESS_MODE_BORDER,
                0.0F, maxAnisotropy,
                D3D12_COMPARISON_FUNC_LESS_EQUAL,
                D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK);
        }

        D3D12_STATIC_SAMPLER_DESC anisotropicMirrorOnce(UINT shaderRegister, UINT maxAnisotropy)
        {
            return CD3DX12_STATIC_SAMPLER_DESC(
                shaderRegister,
                D3D12_FILTER_ANISOTROPIC,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
                D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
                0.0f, maxAnisotropy);
        }
    }
}
