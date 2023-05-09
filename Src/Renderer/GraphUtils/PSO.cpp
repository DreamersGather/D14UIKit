#include "Common/Precompile.h"

#include "Renderer/GraphUtils/PSO.h"

namespace d14engine::renderer::graph_utils
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDescTemplate()
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

        // Required
        psoDesc.pRootSignature = nullptr;
        psoDesc.VS = {};
        psoDesc.PS = {};
        psoDesc.InputLayout = {};

        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;
        psoDesc.SampleDesc.Quality = 0;

        return psoDesc;
    }
}
