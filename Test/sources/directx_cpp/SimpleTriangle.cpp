#define AGILITY_SDK false
#define DX_COMPILER false

#if AGILITY_SDK
#define AGILITY_VER 615
#ifdef _WIN64
#define AGILITY_PATH "library/DirectX/x64/"
#else
#define AGILITY_PATH "library/DirectX/x86/"
#endif
#endif
// Refer to https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/#header-include-order
// It is recommended to include Agility SDK headers before the Windows 10 SDK to avoid conflicts.
#include "DirectX-Header.h"

#include <wrl.h>

using Microsoft::WRL::ComPtr;

#if DX_COMPILER
ComPtr<IDxcUtils> dxcUtils;
ComPtr<IDxcCompiler3> dxcCompiler;
ComPtr<IDxcIncludeHandler> dxcDefaultIncludeHandler;
#endif

#include "Application.h"
#include "Callback.h"
#include "Label.h"
#include "MainWindow.h"
#include "RenderObject.h"
#include "ScenePanel.h"

using namespace d14uikit;

#define DEMO_NAME L"SimpleTriangle"

void buildScene(Renderer* rndr, ScenePanel* sp);

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI") == 0)
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);
    app.setMinSize(app.size());
    app.setResizable(true);

#if DX_COMPILER
    //////////////////////////
    // Initialize DXC Tools //
    //////////////////////////
    {
        std::wstring exePath(MAX_PATH, 0);
        GetModuleFileName(nullptr, exePath.data(), MAX_PATH);
        // There is no need to check the result of find_last_of
        // because the exe-path is guaranteed to contain a "\".
        exePath.resize(exePath.find_last_of(L'\\') + 1);

        // Set this before calling AddDllDirectory to ensure that
        // subsequent LoadLibrary calls search user-defined paths.
        SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
#ifdef _WIN64
        std::wstring libPath = L"library/DirectXShaderCompiler/x64";
#else
        std::wstring libPath = L"library/DirectXShaderCompiler/x86";
#endif
        AddDllDirectory((exePath + libPath).c_str());

        THROW_IF_FAILED(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils)));
        THROW_IF_FAILED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler)));
        THROW_IF_FAILED(dxcUtils->CreateDefaultIncludeHandler(&dxcDefaultIncludeHandler));
    }
#endif
    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);

    ScenePanel scene;
    mwnd.setContent(&scene);

    /////////////////////////
    // Build-Scene  START  //
    /////////////////////////

    try
    {
        buildScene(app.dx12Renderer(), &scene);
    }
    catch (std::runtime_error& e)
    {
        MessageBoxA(nullptr, e.what(), "std::runtime_error", MB_OK | MB_ICONERROR);
    }

    /////////////////////////
    // Build-Scene   END   //
    /////////////////////////

    scene.setAnimState(true);

    Label fpsLbl(L"FPS: None");
    fpsLbl.setParent(&scene);
    fpsLbl.setPosition({ 22, 22 });

    //------------------------------------------- Set UI event callbacks.

     // This will be called before the renderer drawing each frame.
    fpsLbl.D14_onUpdate(p, &)
    {
        static int fps = 0;
        if (app.fps() != fps)
        {
            fps = app.fps();
            ((Label*)p)->setText(L"FPS: " + std::to_wstring(fps));
        }
    };
    return app.run();
}

std::unordered_map<std::wstring, std::shared_ptr<RenderObject>> objects;

struct Vertex
{
    struct Position
    {
        float x;
        float y;
        float z;
        float w;
    }
    position;

    struct Color
    {
        float r;
        float g;
        float b;
        float a;
    }
    color;
};

void buildScene(Renderer* rndr, ScenePanel* sp)
{
    ////////////////////////
    // Vertices & Indices //
    ////////////////////////

    /************/
    /* CPU Side */
    /************/

    Vertex vertices[3] =
    {
        /* A */ { /* position */ { +0.0f, +0.5f, 0.0f, 1.0f }, /* color */ { 1.0f, 0.0f, 0.0f, 1.0f } },
        /* B */ { /* position */ { +0.5f, -0.5f, 0.0f, 1.0f }, /* color */ { 0.0f, 1.0f, 0.0f, 1.0f } },
        /* C */ { /* position */ { -0.5f, -0.5f, 0.0f, 1.0f }, /* color */ { 0.0f, 0.0f, 1.0f, 1.0f } },
    }; 
    UINT32 indices[3] = { 0, 1, 2 };

    /************/
    /* GPU Side */
    /************/

    ComPtr<ID3D12Resource> vertexResource = {};
    ComPtr<ID3D12Resource> vertexIntermediate = {};

    ComPtr<ID3D12Resource> indexResource = {};
    ComPtr<ID3D12Resource> indexIntermediate = {};

    D3D12_HEAP_PROPERTIES heapProp = {};
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProp.CreationNodeMask = 1;
    heapProp.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // Resource Buffers

    heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
    resourceDesc.Width = sizeof(vertices);
    {
        THROW_IF_FAILED(rndr->d3d12Device()->CreateCommittedResource
        (
            /* pHeapProperties      */ &heapProp,
            /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
            /* pDesc                */ &resourceDesc,
            /* InitialResourceState */ D3D12_RESOURCE_STATE_GENERIC_READ,
            /* pOptimizedClearValue */ nullptr,
            /* riidResource         */
            /* ppvResource          */ IID_PPV_ARGS(&vertexResource)
        ));
    }
    heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
    resourceDesc.Width = sizeof(indices);
    {
        THROW_IF_FAILED(rndr->d3d12Device()->CreateCommittedResource
        (
            /* pHeapProperties      */ &heapProp,
            /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
            /* pDesc                */ &resourceDesc,
            /* InitialResourceState */ D3D12_RESOURCE_STATE_GENERIC_READ,
            /* pOptimizedClearValue */ nullptr,
            /* riidResource         */
            /* ppvResource          */ IID_PPV_ARGS(&indexResource)
        ));
    }

    // Intermediate Buffers

    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    resourceDesc.Width = sizeof(vertices);
    {
        THROW_IF_FAILED(rndr->d3d12Device()->CreateCommittedResource
        (
            /* pHeapProperties      */ &heapProp,
            /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
            /* pDesc                */ &resourceDesc,
            /* InitialResourceState */ D3D12_RESOURCE_STATE_GENERIC_READ,
            /* pOptimizedClearValue */ nullptr,
            /* riidResource         */
            /* ppvResource          */ IID_PPV_ARGS(&vertexIntermediate)
        ));
    }
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    resourceDesc.Width = sizeof(indices);
    {
        THROW_IF_FAILED(rndr->d3d12Device()->CreateCommittedResource
        (
            /* pHeapProperties      */ &heapProp,
            /* HeapFlags            */ D3D12_HEAP_FLAG_NONE,
            /* pDesc                */ &resourceDesc,
            /* InitialResourceState */ D3D12_RESOURCE_STATE_GENERIC_READ,
            /* pOptimizedClearValue */ nullptr,
            /* riidResource         */
            /* ppvResource          */ IID_PPV_ARGS(&indexIntermediate)
        ));
    }

    /***************/
    /* Upload Data */
    /***************/

    rndr->beginGpuCommand();

    // Copy from CPU to intermediate buffer.

    BYTE* vertexMapped = nullptr;
    {
        THROW_IF_FAILED(vertexIntermediate->Map(0, nullptr, (void**)&vertexMapped));

        memcpy(vertexMapped, vertices, sizeof(vertices));

        vertexIntermediate->Unmap(0, nullptr);
    }
    BYTE* indexMapped = nullptr;
    {
        THROW_IF_FAILED(indexIntermediate->Map(0, nullptr, (void**)&indexMapped));

        memcpy(indexMapped, indices, sizeof(indices));

        indexIntermediate->Unmap(0, nullptr);
    }

    // Copy from intermediate buffer to GPU.

    D3D12_RESOURCE_BARRIER barriers[2] = {};
    for (auto& barrier : barriers)
    {
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
    }
    barriers[0].Transition.pResource = vertexResource.Get();
    barriers[1].Transition.pResource = indexResource.Get();

    rndr->cmdList()->ResourceBarrier(2, barriers);

    rndr->cmdList()->CopyBufferRegion
    (
        /* pDstBuffer */ vertexResource.Get(),
        /* DstOffset  */ 0,
        /* pSrcBuffer */ vertexIntermediate.Get(),
        /* SrcOffset  */ 0,
        /* NumBytes   */ sizeof(vertices)
    );
    rndr->cmdList()->CopyBufferRegion
    (
        /* pDstBuffer */ indexResource.Get(),
        /* DstOffset  */ 0,
        /* pSrcBuffer */ indexIntermediate.Get(),
        /* SrcOffset  */ 0,
        /* NumBytes   */ sizeof(indices)
    );

    for (auto& barrier : barriers)
    {
        std::swap
        (
            barrier.Transition.StateBefore,
            barrier.Transition.StateAfter
        );
    }
    rndr->cmdList()->ResourceBarrier(2, barriers);

    rndr->endGpuCommand();

    //////////////////
    // HLSL Shaders //
    //////////////////

#if DX_COMPILER
    ComPtr<IDxcBlob> vs = {};
    ComPtr<IDxcBlob> ps = {};
    {
#ifdef _DEBUG
#define SHADER_FLAGS DXC_ARG_DEBUG, DXC_ARG_SKIP_OPTIMIZATIONS
#else
#define SHADER_FLAGS DXC_ARG_OPTIMIZATION_LEVEL3
#endif
        auto hlslFilePath = L"sources/directx_cpp/SimpleTriangle.hlsl";

        ComPtr<IDxcBlobEncoding> hlslFile = {};
        THROW_IF_FAILED(dxcUtils->LoadFile(hlslFilePath, nullptr, &hlslFile));

        DxcBuffer source = {};
        source.Ptr = hlslFile->GetBufferPointer();
        source.Size = hlslFile->GetBufferSize();
        source.Encoding = DXC_CP_ACP;

#pragma warning(push)
#pragma warning(disable : 6387)

        // Vertex Shader
        {
            LPCWSTR arguments[] =
            {
                hlslFilePath, L"-E", L"VS", L"-T", L"vs_6_0",
                SHADER_FLAGS, DXC_ARG_PACK_MATRIX_ROW_MAJOR
            };
            ComPtr<IDxcResult> result = {};
            THROW_IF_FAILED(dxcCompiler->Compile
            (
                /* pSource         */ &source,
                /* pArguments      */ arguments,
                /* argCount        */ 5,
                /* pIncludeHandler */ dxcDefaultIncludeHandler.Get(),
                /* riid            */
                /* ppResult        */ IID_PPV_ARGS(&result)
            ));
            // Check Compilation Errors
            {
                ComPtr<IDxcBlobUtf8> error = {};
                THROW_IF_FAILED(result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&error), nullptr));

                if (error && error->GetStringLength() != 0)
                {
                    OutputDebugStringA((char*)error->GetStringPointer());
                }
            }
            THROW_IF_FAILED(result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&vs), nullptr));
        }

        // Pixel Shader
        {
            LPCWSTR arguments[] =
            {
                hlslFilePath, L"-E", L"PS", L"-T", L"ps_6_0",
                SHADER_FLAGS, DXC_ARG_PACK_MATRIX_ROW_MAJOR
            };
            ComPtr<IDxcResult> result = {};
            THROW_IF_FAILED(dxcCompiler->Compile
            (
                /* pSource         */ &source,
                /* pArguments      */ arguments,
                /* argCount        */ 5,
                /* pIncludeHandler */ dxcDefaultIncludeHandler.Get(),
                /* riid            */
                /* ppResult        */ IID_PPV_ARGS(&result)
            ));
            // Check Compilation Errors
            {
                ComPtr<IDxcBlobUtf8> error;
                THROW_IF_FAILED(result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&error), nullptr));

                if (error && error->GetStringLength() != 0)
                {
                    OutputDebugStringA((char*)error->GetStringPointer());
                }
            }
            THROW_IF_FAILED(result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&ps), nullptr));
        }
#pragma warning(pop)
    }
#else
    ComPtr<ID3DBlob> vs = {};
    ComPtr<ID3DBlob> ps = {};
    {
#ifdef _DEBUG
#define SHADER_FLAGS D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION
#else
#define SHADER_FLAGS D3DCOMPILE_OPTIMIZATION_LEVEL3
#endif
        HRESULT hr = S_OK;
        ComPtr<ID3DBlob> error = {};

        // Vertex Shader
        hr = D3DCompileFromFile
        (
            /* pFileName   */ L"sources/directx_cpp/SimpleTriangle.hlsl",
            /* pDefines    */ nullptr,
            /* pInclude    */ nullptr,
            /* pEntrypoint */ "VS",
            /* pTarget     */ "vs_5_0",
            /* Flags1      */ SHADER_FLAGS,
            /* Flags2      */ 0,
            /* ppCode      */ &vs,
            /* ppErrorMsgs */ &error
        );
        if (error)
        {
            OutputDebugStringA((char*)error->GetBufferPointer());
        }
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to compile vertex shader!");
        }

        // Pixel Shader
        hr = D3DCompileFromFile
        (
            /* pFileName   */ L"sources/directx_cpp/SimpleTriangle.hlsl",
            /* pDefines    */ nullptr,
            /* pInclude    */ nullptr,
            /* pEntrypoint */ "PS",
            /* pTarget     */ "ps_5_0",
            /* Flags1      */ SHADER_FLAGS,
            /* Flags2      */ 0,
            /* ppCode      */ &ps,
            /* ppErrorMsgs */ &error
        );
        if (error)
        {
            OutputDebugStringA((char*)error->GetBufferPointer());
        }
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to compile pixel shader!");
        }
    }
#endif

    ////////////////////
    // Root Signature //
    ////////////////////

    ComPtr<ID3D12RootSignature> rootSignature;
    {
        D3D12_ROOT_SIGNATURE_DESC desc = {};
        desc.NumParameters = 0;
        desc.pParameters = nullptr;
        desc.NumStaticSamplers = 0;
        desc.pStaticSamplers = nullptr;
        desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        ComPtr<ID3DBlob> blob = {};
        ComPtr<ID3DBlob> error = {};
        HRESULT hr = D3D12SerializeRootSignature
        (
            /* pRootSignature */ &desc,
            /* Version        */ D3D_ROOT_SIGNATURE_VERSION_1,
            /* ppBlob         */ &blob,
            /* ppErrorBlob    */ &error
        );
        if (error)
        {
            OutputDebugStringA((char*)error->GetBufferPointer());
        }
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to serialize root signature!");
        }
        THROW_IF_FAILED(rndr->d3d12Device()->CreateRootSignature
        (
            /* nodeMask               */ 0,
            /* pBlobWithRootSignature */ blob->GetBufferPointer(),
            /* blobLengthInBytes      */ blob->GetBufferSize(),
            /* riid                   */
            /* ppvRootSignature       */IID_PPV_ARGS(&rootSignature)
        ));
    }

    /////////////////////////////
    // Graphics Pipeline State //
    /////////////////////////////

    ComPtr<ID3D12PipelineState> pipelineState;
    {
        D3D12_INPUT_ELEMENT_DESC inputElemDescs[] =
        {
            {
                /* SemanticName         */ "POSITION",
                /* SemanticIndex        */ 0,
                /* Format               */ DXGI_FORMAT_R32G32B32A32_FLOAT,
                /* InputSlot            */ 0,
                /* AlignedByteOffset    */ 0,
                /* InputSlotClass       */ D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
                /* InstanceDataStepRate */ 0
            },
            {
                /* SemanticName         */ "COLOR",
                /* SemanticIndex        */ 0,
                /* Format               */ DXGI_FORMAT_R32G32B32A32_FLOAT,
                /* InputSlot            */ 0,
                /* AlignedByteOffset    */ D3D12_APPEND_ALIGNED_ELEMENT,
                /* InputSlotClass       */ D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
                /* InstanceDataStepRate */ 0
            }
        };
        D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};
        {
            // Root Signature
            pipelineStateDesc.pRootSignature = rootSignature.Get();

            // Vertex Shader
            pipelineStateDesc.VS.pShaderBytecode = vs->GetBufferPointer();
            pipelineStateDesc.VS.BytecodeLength = vs->GetBufferSize();

            // Pixel Shader
            pipelineStateDesc.PS.pShaderBytecode = ps->GetBufferPointer();
            pipelineStateDesc.PS.BytecodeLength = ps->GetBufferSize();

            // Blend Operation
            auto& blendTarget = pipelineStateDesc.BlendState.RenderTarget[0];

            blendTarget.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

            //blendTarget.RenderTargetWriteMask =
            //    D3D12_COLOR_WRITE_ENABLE_RED |
            //    D3D12_COLOR_WRITE_ENABLE_GREEN |
            //    D3D12_COLOR_WRITE_ENABLE_BLUE |
            //    D3D12_COLOR_WRITE_ENABLE_ALPHA;

            // Rasterization
            pipelineStateDesc.SampleMask = UINT_MAX;
            pipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
            pipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
            pipelineStateDesc.RasterizerState.FrontCounterClockwise = FALSE;

            // Input Layout
            pipelineStateDesc.InputLayout.pInputElementDescs = inputElemDescs;
            pipelineStateDesc.InputLayout.NumElements = 2;

            // Miscellaneous
            pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            pipelineStateDesc.NumRenderTargets = 1;
            pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
            pipelineStateDesc.SampleDesc.Count = 1;
            pipelineStateDesc.SampleDesc.Quality = 0;
        }
        THROW_IF_FAILED(rndr->d3d12Device()->CreateGraphicsPipelineState
        (
            /* pDesc           */ &pipelineStateDesc,
            /* riid            */
            /* ppPipelineState */ IID_PPV_ARGS(&pipelineState)
        ));
    }

    ////////////////////
    // Draw Triangle! //
    ////////////////////

    auto viewport = std::make_shared<D3D12_VIEWPORT>();
    viewport->TopLeftX = 0.0f;
    viewport->TopLeftY = 0.0f;
    viewport->Width = (FLOAT)sp->width();
    viewport->Height = (FLOAT)sp->height();
    viewport->MinDepth = 0.0f;
    viewport->MaxDepth = 1.0f;

    auto scissors = std::make_shared<D3D12_RECT>();
    scissors->left = 0;
    scissors->right = sp->width();
    scissors->top = 0;
    scissors->bottom = sp->height();

    sp->D14_onSize(p, e, =)
    {
        viewport->Width = (FLOAT)e->size().width;
        viewport->Height = (FLOAT)e->size().height;

        scissors->right = e->size().width;
        scissors->bottom = e->size().height;
    };

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
    vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
    vertexBufferView.SizeInBytes = sizeof(vertices);
    vertexBufferView.StrideInBytes = sizeof(Vertex);

    D3D12_INDEX_BUFFER_VIEW indexBufferView = {};
    indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
    indexBufferView.SizeInBytes = sizeof(indices);
    indexBufferView.Format = DXGI_FORMAT_R32_UINT;

    auto triangle = std::make_shared<RenderObject>();

    sp->addObject(triangle.get());
    objects[L"triangle"] = triangle;

    triangle->callback().draw =
    [
        rndr,

        viewport,
        scissors,

        rootSignature,

        pipelineState,

        vertexResource,
        vertexBufferView,

        indexResource,
        indexBufferView
    ]
    (RenderObject* obj)
    {
        rndr->cmdList()->RSSetViewports(1, viewport.get());

        rndr->cmdList()->RSSetScissorRects(1, scissors.get());

        rndr->cmdList()->SetGraphicsRootSignature(rootSignature.Get());

        rndr->cmdList()->SetPipelineState(pipelineState.Get());

        rndr->cmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        rndr->cmdList()->IASetVertexBuffers
        (
            /* StartSlot */ 0,
            /* NumViews  */ 1,
            /* pViews    */ &vertexBufferView
        );
        rndr->cmdList()->IASetIndexBuffer(&indexBufferView);

        rndr->cmdList()->DrawIndexedInstanced
        (
            /* IndexCountPerInstance */ 3,
            /* InstanceCount         */ 1,
            /* StartIndexLocation    */ 0,
            /* BaseVertexLocation    */ 0,
            /* StartInstanceLocation */ 0
        );
    };
}
