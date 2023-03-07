#pragma once

#include "Common/Precompile.h"

#include "Common/CppLangUtils/PasskeyIdiom.h"
#include "Renderer/FrameResource.h"
#include "Renderer/Interfaces/ICamera.h"

namespace d14engine::renderer
{
    struct DefaultBuffer;
    struct Renderer;

    struct Letterbox : PasskeyIdiom<Renderer>
    {
        friend struct Renderer;

        Letterbox(Renderer* rndr, Token);

        // prevent std::unique_ptr from generating default deleter
        ~Letterbox();

    private:
        Renderer* rndr = nullptr;

        FrameResource::CmdAllocArray m_cmdAllocs = {};

    private:
        bool m_enabled = false;

    public:
        bool enabled() const;
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
        XMVECTORF32 color = Colors::Blue;

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

    public:
        void present();

    private:
        void copySceneToBackBuffer();
        void postSceneToBackBuffer();
    };
}
