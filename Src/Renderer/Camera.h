#pragma once

#include "Common/Precompile.h"

#include "Common/MathUtils/3D.h"

#include "Renderer/FrameResource.h"
#include "Renderer/Interfaces/DrawObject.h"
#include "Renderer/Interfaces/ICamera.h"

namespace d14engine::renderer
{
    struct ConstantBuffer;
    struct Renderer;

    struct Camera : ICamera, DrawObject
    {
        explicit Camera(ID3D12Device* device);

        // prevent std::unique_ptr from generating default deleter
        virtual ~Camera() = default;

        Viewport viewport() const override;

        Scissors scissors() const override;

        void onViewResize(UINT viewWidth, UINT viewHeight) override;

        void onRendererUpdateObjectHelper(Renderer* rndr) override;

        void onRendererDrawD3d12ObjectHelper(Renderer* rndr) override;

    public:
        XMFLOAT3 eyePos = { 0.0f, 0.0f, 0.0f };
        XMFLOAT3 eyeDir = { 0.0f, 0.0f, 1.0f };

        XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };

    protected:
        Viewport m_viewport = {};
        Scissors m_scissors = {};

    public:
        float fovAngleY = XM_PIDIV4; // in radians

        Optional<float> aspectRatio = {};

        // Returns Viewport-Width / Viewport-Height if aspectRatio is empty.
        float getAspectRatio() const;

        float nearZ = 1.0f, farZ = 1000.0f;

    protected:
        struct Data
        {
            XMFLOAT4X4 viewMatrix = math_utils::identityFloat4x4();
            XMFLOAT4X4 projMatrix = math_utils::identityFloat4x4();
        }
        m_data = {};

    public:
        XMMATRIX XM_CALLCONV viewMatrix() const;
        XMMATRIX XM_CALLCONV projMatrix() const;

        void updateViewMatrix();
        void updateProjMatrix();

    public:
        UINT rootParamIndex = 0;

        // Indicates how many buffers need to be updated in the subsequent render passes.
        UINT dirtyFrameCount = FrameResource::g_bufferCount;

    protected:
        using BufferArray = FrameResource::Array<UniquePtr<ConstantBuffer>>;

        BufferArray m_buffers = {};

    public:
        const BufferArray& buffers() const;
    };
}
