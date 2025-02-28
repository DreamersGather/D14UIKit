#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/PasskeyIdiom.h"

#include "Renderer/FrameData/FrameResource.h"
#include "Renderer/Interfaces/ICamera.h"

namespace d14engine::renderer
{
    struct DefaultBuffer;
    struct Renderer;

    struct Letterbox : PasskeyIdiom<Renderer>
    {
        friend struct Renderer;

        Letterbox(Renderer* rndr, Token);

        // A std::unique_ptr<T> member needs to obtain sizeof(T) to call the default deleter.
        // In the case of forward declaration, T is an incomplete type, so it cannot compile.
        //
        // The solution is to declare the destructor in the header file and
        // implement it in the source file (where T is a complete type).
        //
        // PS: For MSVC, directly defining dtor() = default in the header file can also compile,
        // but for the sake of standardization, it is better to define it in the source file.
        virtual ~Letterbox();

    private:
        Renderer* rndr = nullptr;

        FrameResource::CmdAllocArray m_cmdAllocs = {};

        ID3D12CommandAllocator* cmdAlloc() const;

    private:
        bool m_enabled = false;

    public:
        bool enabled() const;
        // GPU Commands Required
        void setEnabled(bool value);

    private:
        ICamera::Viewport m_viewport = {};
        ICamera::Scissors m_scissors = {};

    public:
        void resize(
            UINT sceneWidth,
            UINT sceneHeight,
            UINT windowWidth,
            UINT windowHeight);

    public:
        XMVECTORF32 color = Colors::Black;

    private:
        ComPtr<ID3D12RootSignature> m_rootSigature = {};
        ComPtr<ID3D12PipelineState> m_pipelineState = {};

        struct Vertex
        {
            XMFLOAT4 position = {};
            XMFLOAT2 texcoord = {};
        };

        UniquePtr<DefaultBuffer> m_vertexBuffer = {};
        D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView = {};

    private:
        void createRootSignature();
        void createPipelineState();

        // GPU Commands Required
        void createVertexBuffer();

    public:
        void present();

    private:
        void copySceneToBackBuffer();
        void postSceneToBackBuffer();
    };
}
