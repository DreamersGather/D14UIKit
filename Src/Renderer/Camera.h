#pragma once

#include "Common/Precompile.h"

#include "Common/MathUtils/3D.h"

#include "Renderer/FrameResource.h"
#include "Renderer/Interfaces/ICamera.h"

namespace d14engine::renderer
{
    struct ConstantBuffer;
    struct Renderer;

    struct Camera : ICamera
    {
        explicit Camera(Renderer* rndr);

    public:
        // IDrawObject
        bool isD3d12ObjectVisible() const override;

        void setD3d12ObjectVisible(bool value) override;

        void onRendererUpdateObject(Renderer* rndr) override;

        void onRendererDrawD3d12Object(Renderer* rndr) override;

        // ICamera
        Viewport viewport() const override;

        Scissors scissors() const override;

        void onViewResize(UINT viewWidth, UINT viewHeight) override;

    protected:
        bool m_visible = true;

        XMFLOAT3 m_position = { 0.0f, 0.0f, 0.0f };

        XMFLOAT3 m_right = { 1.0f, 0.0f, 0.0f };
        XMFLOAT3 m_up = { 0.0f, 1.0f, 0.0f };
        XMFLOAT3 m_look = { 0.0f, 0.0f, 1.0f };

    public:
        XMVECTOR XM_CALLCONV position() const;
        void XM_CALLCONV setPosition(FXMVECTOR pos);

        XMVECTOR XM_CALLCONV right() const;
        XMVECTOR XM_CALLCONV up() const;
        XMVECTOR XM_CALLCONV look() const;
        void XM_CALLCONV setView(FXMVECTOR right, FXMVECTOR up, FXMVECTOR look);

    protected:
        Viewport m_viewport = {};
        Scissors m_scissors = {};

        float m_fovAngleY = XM_PIDIV4; // in radians

        Optional<float> m_aspectRatio = {};

        float m_nearZ = 1.0f, m_farZ = 1000.0f;

    public:
        float fovAngleY() const;
        void setFovAngleY(float rad);

        Optional<float> aspectRatio() const;
        void setAspectRatio(OptParam<float> value);

        // Returns Viewport-Width / Viewport-Height if m_aspectRatio is empty.
        float getAspectRatio() const;

        float nearZ() const; float farZ() const;
        void setNearFarZ(float nearZ, float farZ);

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

    protected:
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
