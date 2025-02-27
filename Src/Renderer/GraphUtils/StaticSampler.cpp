#include "Common/Precompile.h"

#include "Renderer/GraphUtils/StaticSampler.h"

namespace d14engine::renderer::graph_utils
{
    namespace static_sampler
    {
        const D3D12_STATIC_SAMPLER_DESC* pointWrap(UINT shaderRegister)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_MIN_MAG_MIP_POINT,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_WRAP
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* pointMirror(UINT shaderRegister)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_MIN_MAG_MIP_POINT,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* pointClamp(UINT shaderRegister)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_MIN_MAG_MIP_POINT,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_CLAMP
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* pointBorder(UINT shaderRegister)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_MIN_MAG_MIP_POINT,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            /* mipLODBias     */ 0.0F,
            /* maxAnisotropy  */ 16U,
            /* comparisonFunc */ D3D12_COMPARISON_FUNC_LESS_EQUAL,
            /* borderColor    */ D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* pointMirrorOnce(UINT shaderRegister)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_MIN_MAG_MIP_POINT,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* linearWrap(UINT shaderRegister)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_MIN_MAG_MIP_LINEAR,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_WRAP
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* linearMirror(UINT shaderRegister)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_MIN_MAG_MIP_LINEAR,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* linearClamp(UINT shaderRegister)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_MIN_MAG_MIP_LINEAR,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_CLAMP
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* linearBorder(UINT shaderRegister)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_MIN_MAG_MIP_LINEAR,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            /* mipLODBias     */ 0.0F,
            /* maxAnisotropy  */ 16U,
            /* comparisonFunc */ D3D12_COMPARISON_FUNC_LESS_EQUAL,
            /* borderColor    */ D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* linearMirrorOnce(UINT shaderRegister)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_MIN_MAG_MIP_LINEAR,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* anisotropicWrap(UINT shaderRegister, UINT maxAnisotropy)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_ANISOTROPIC,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_WRAP,
            /* mipLODBias     */ 0.0F,
            /* maxAnisotropy  */ maxAnisotropy
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* anisotropicMirror(UINT shaderRegister, UINT maxAnisotropy)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_ANISOTROPIC,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
            /* mipLODBias     */ 0.0F,
            /* maxAnisotropy  */ maxAnisotropy
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* anisotropicClamp(UINT shaderRegister, UINT maxAnisotropy)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_ANISOTROPIC,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
            /* mipLODBias     */ 0.0F,
            /* maxAnisotropy  */ maxAnisotropy
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* anisotropicBorder(UINT shaderRegister, UINT maxAnisotropy)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_ANISOTROPIC,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_BORDER,
            /* mipLODBias     */ 0.0F,
            /* maxAnisotropy  */ maxAnisotropy,
            /* comparisonFunc */ D3D12_COMPARISON_FUNC_LESS_EQUAL,
            /* borderColor    */ D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK
            );
            return &desc;
        }

        const D3D12_STATIC_SAMPLER_DESC* anisotropicMirrorOnce(UINT shaderRegister, UINT maxAnisotropy)
        {
            const static auto desc =
            CD3DX12_STATIC_SAMPLER_DESC
            (
            /* shaderRegister */ shaderRegister,
            /* filter         */ D3D12_FILTER_ANISOTROPIC,
            /* addressU       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
            /* addressV       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
            /* addressW       */ D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE,
            /* mipLODBias     */ 0.0F,
            /* maxAnisotropy  */ maxAnisotropy
            );
            return &desc;
        }
    }
}
