#include "Common/Precompile.h"

#include "Renderer/Letterbox.h"

#include "Common/DirectXError.h"
#include "Common/ResourcePack.h"

#include "Renderer/GpuBuffer.h"
#include "Renderer/GraphUtils/Barrier.h"
#include "Renderer/GraphUtils/ParamHelper.h"
#include "Renderer/GraphUtils/PSO.h"
#include "Renderer/GraphUtils/StaticSampler.h"
#include "Renderer/Renderer.h"

namespace d14engine::renderer
{
    Letterbox::Letterbox(Renderer* rndr, Token) : rndr(rndr)
    {
        for (auto& cmdAlloc : m_cmdAllocs)
        {
            THROW_IF_FAILED(rndr->d3d12Device()->CreateCommandAllocator
            (
            /* type               */ D3D12_COMMAND_LIST_TYPE_DIRECT,
            /* riid               */
            /* ppCommandAllocator */ IID_PPV_ARGS(&cmdAlloc)
            ));
        }
    }

    Letterbox::~Letterbox() = default;

    ID3D12CommandAllocator* Letterbox::cmdAlloc() const
    {
        return m_cmdAllocs.at(rndr->currFrameIndex()).Get();
    }

    bool Letterbox::enabled() const
    {
        return m_enabled;
    }

    void Letterbox::setEnabled(bool value)
    {
        if (m_enabled != value)
        {
            if (m_enabled = value)
            {
                createRootSignature();
                createPipelineState();
                createVertexBuffer();
            }
            else // Clear created components.
            {
                m_rootSigature.Reset();
                m_pipelineState.Reset();

                m_vertexBuffer.reset();
                m_vertexBufferView = {};
            }
        }
    }

    void Letterbox::resize(
        UINT sceneWidth,
        UINT sceneHeight,
        UINT windowWidth,
        UINT windowHeight)
    {
        auto horzRatio = (float)sceneWidth / (float)windowWidth;
        auto vertRatio = (float)sceneHeight / (float)windowHeight;

        auto x = 1.0f;
        auto y = 1.0f;

        if (horzRatio < vertRatio)
        {
            x = horzRatio / vertRatio;
        }
        else // fit width
        {
            y = vertRatio / horzRatio;
        }

        m_viewport.TopLeftX = (float)windowWidth * (1.0f - x) * 0.5f;
        m_viewport.TopLeftY = (float)windowHeight * (1.0f - y) * 0.5f;
        m_viewport.Width = x * (float)windowWidth;
        m_viewport.Height = y * (float)windowHeight;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;

        m_scissors.left = (LONG)m_viewport.TopLeftX;
        m_scissors.right = (LONG)(m_viewport.TopLeftX + m_viewport.Width);
        m_scissors.top = (LONG)m_viewport.TopLeftY;
        m_scissors.bottom = (LONG)(m_viewport.TopLeftY + m_viewport.Height);
    }

    void Letterbox::createRootSignature()
    {
        CD3DX12_DESCRIPTOR_RANGE1 descRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

        CD3DX12_ROOT_PARAMETER1 rootParam = {};
        rootParam.InitAsDescriptorTable(1, &descRange, D3D12_SHADER_VISIBILITY_PIXEL);

        D3D12_ROOT_SIGNATURE_FLAGS rootSigFlags =
        (
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |

            // Only VS and PS are needed to resample the scene texture.
            D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
        );
        CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSigDesc = {};
        rootSigDesc.Init_1_1
        (
        /* numParameters     */ 1,
        /* _pParameters      */ &rootParam,
        /* numStaticSamplers */ 1,
        /* _pStaticSamplers  */ graph_utils::static_sampler::linearBorder(0),
        /* flags             */ rootSigFlags
        );
        ComPtr<ID3DBlob> rootSigBlob = {};

        THROW_IF_ERROR(D3DX12SerializeVersionedRootSignature
        (
        /* pRootSignatureDesc */ &rootSigDesc,
        /* MaxVersion         */ rndr->d3d12DeviceInfo().feature.rootSignature.HighestVersion,
        /* ppBlob             */ &rootSigBlob,
        /* ppErrorBlob        */ ppErrorBlob
        ));
        THROW_IF_FAILED(rndr->d3d12Device()->CreateRootSignature
        (
        /* nodeMask               */ 0,
        /* pBlobWithRootSignature */ 
        /* blobLengthInBytes      */ BLB_PSZ_ARGS(rootSigBlob),
        /* riid                   */
        /* ppvRootSignature       */ IID_PPV_ARGS(&m_rootSigature)
        ));
    }

    void Letterbox::createPipelineState()
    {
        auto VS = loadResource(L"VS", L"Shader");
        auto PS = loadResource(L"PS", L"Shader");

        D3D12_INPUT_ELEMENT_DESC inputElemDescs[] =
        {{
            .SemanticName         = "POSITION",
            .SemanticIndex        = 0,
            .Format               = DXGI_FORMAT_R32G32B32A32_FLOAT,
            .InputSlot            = 0,
            .AlignedByteOffset    = 0,
            .InputSlotClass       = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
            .InstanceDataStepRate = 0
        },{
            .SemanticName         = "TEXCOORD",
            .SemanticIndex        = 0,
            .Format               = DXGI_FORMAT_R32G32_FLOAT,
            .InputSlot            = 0,
            .AlignedByteOffset    = D3D12_APPEND_ALIGNED_ELEMENT,
            .InputSlotClass       = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
            .InstanceDataStepRate = 0
        }};

        auto psoDesc = graph_utils::GPSODescTemplate();

        psoDesc.pRootSignature = m_rootSigature.Get();
        psoDesc.VS = { VS.data, VS.size };
        psoDesc.PS = { PS.data, PS.size };
        psoDesc.InputLayout = { ARR_NUM_ARGS(inputElemDescs) };
        psoDesc.DepthStencilState.DepthEnable = FALSE;

        THROW_IF_FAILED(rndr->d3d12Device()->CreateGraphicsPipelineState
        (
        /* pDesc           */ &psoDesc,
        /* riid            */
        /* ppPipelineState */ IID_PPV_ARGS(&m_pipelineState)
        ));
    }

    void Letterbox::createVertexBuffer()
    {
        Vertex quad[] =
        {
        /* Top    Left  */ { { -1.0f, +1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        /* Top    Right */ { { +1.0f, +1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        /* Bottom Left  */ { { -1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
        /* Bottom Right */ { { +1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }
        };
        m_vertexBuffer = std::make_unique<DefaultBuffer>(rndr->d3d12Device(), sizeof(quad));
        m_vertexBuffer->uploadData(rndr->cmdList(), quad, sizeof(quad));

        m_vertexBufferView.BufferLocation = m_vertexBuffer->resource()->GetGPUVirtualAddress();
        m_vertexBufferView.SizeInBytes = sizeof(quad);
        m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    }

    void Letterbox::present()
    {
        rndr->resetCmdList(cmdAlloc());

        if (m_enabled)
        {
            postSceneToBackBuffer();
        }
        else copySceneToBackBuffer();

        rndr->submitCmdList();
    }

#pragma warning(push)
// The specific resource components of the renderer
// are gauranteed to be valid when composition=false.
#pragma warning(disable : 26815)

    void Letterbox::copySceneToBackBuffer()
    {
        auto currBackBuffer = rndr->currBackBuffer().value();
        auto sceneBuffer = rndr->sceneBuffer().value();

        D3D12_RESOURCE_BARRIER barriers[] =
        {
        CD3DX12_RESOURCE_BARRIER::Transition
        (
            currBackBuffer,
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_COPY_DEST
        ),
        CD3DX12_RESOURCE_BARRIER::Transition
        (
            sceneBuffer,
            D3D12_RESOURCE_STATE_COMMON,
            D3D12_RESOURCE_STATE_COPY_SOURCE
        )};
        rndr->cmdList()->ResourceBarrier(NUM_ARR_ARGS(barriers));

        CD3DX12_TEXTURE_COPY_LOCATION dst(currBackBuffer);
        CD3DX12_TEXTURE_COPY_LOCATION src(sceneBuffer);
        CD3DX12_BOX srcBox(0, 0, rndr->getSceneWidth(), rndr->getSceneHeight());

        rndr->cmdList()->CopyTextureRegion(&dst, 0, 0, 0, &src, &srcBox);

        graph_utils::revertBarrier(NUM_ARR_ARGS(barriers));
        rndr->cmdList()->ResourceBarrier(NUM_ARR_ARGS(barriers));
    }

    void Letterbox::postSceneToBackBuffer()
    {
        D3D12_RESOURCE_BARRIER barriers[] =
        {
        CD3DX12_RESOURCE_BARRIER::Transition
        (
            rndr->currBackBuffer().value(),
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET
        ),
        CD3DX12_RESOURCE_BARRIER::Transition
        (
            rndr->sceneBuffer().value(),
            D3D12_RESOURCE_STATE_COMMON,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
        )};
        rndr->cmdList()->ResourceBarrier(NUM_ARR_ARGS(barriers));

        ///////////////////
        // Output Merger //
        ///////////////////

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rndr->backRtvHandle().value();
        rndr->cmdList()->OMSetRenderTargets(1, &rtvHandle, TRUE, nullptr);
        rndr->cmdList()->ClearRenderTargetView(rtvHandle, color, 0, nullptr);

        ////////////////
        // Rasterizer //
        ////////////////

        rndr->cmdList()->RSSetViewports(1, &m_viewport);
        rndr->cmdList()->RSSetScissorRects(1, &m_scissors);

        ////////////////////
        // Shader Context //
        ////////////////////

        rndr->cmdList()->SetGraphicsRootSignature(m_rootSigature.Get());
        rndr->cmdList()->SetPipelineState(m_pipelineState.Get());

        auto srvHeap = rndr->srvHeap().value();
        auto srvHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

        ID3D12DescriptorHeap* ppHeaps[] = { srvHeap };
        rndr->cmdList()->SetDescriptorHeaps(NUM_ARR_ARGS(ppHeaps));
        rndr->cmdList()->SetGraphicsRootDescriptorTable(0, srvHandle);

        /////////////////////
        // Input Assembler //
        /////////////////////

        // We are drawing 2 triangles with 4 vertices (sharing 1 edge).
        rndr->cmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        rndr->cmdList()->IASetVertexBuffers(0, 1, &m_vertexBufferView);

        rndr->cmdList()->DrawInstanced
        (
        /* VertexCountPerInstance */ 4,
        /* InstanceCount          */ 1,
        /* StartVertexLocation    */ 0,
        /* StartInstanceLocation  */ 0
        );
        graph_utils::revertBarrier(NUM_ARR_ARGS(barriers));
        rndr->cmdList()->ResourceBarrier(NUM_ARR_ARGS(barriers)); 
    }
#pragma warning(pop)
}
