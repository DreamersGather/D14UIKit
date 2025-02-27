#pragma once

#include "Common/Precompile.h"

#include "Common/MathUtils/3D.h"

#include "Renderer/FrameData/FrameResource.h"
#include "Renderer/Interfaces/DrawObject.h"
#include "Renderer/Interfaces/ICamera.h"

namespace d14engine::renderer
{
    struct ConstantBuffer;
    struct Renderer;

    struct Camera : ICamera, DrawObject
    {
        explicit Camera(ID3D12Device* device);

        // A std::unique_ptr<T> member needs to obtain sizeof(T) to call the default deleter.
        // In the case of forward declaration, T is an incomplete type, so it cannot compile.
        // 
        // The solution is to declare the destructor in the header file and
        // implement it in the source file (where T is a complete type).
        // 
        // PS: For MSVC, directly defining dtor() = default in the header file can also compile,
        // but for the sake of standardization, it is better to define it in the source file.
        virtual ~Camera();

        /////////////
        // ICamera //
        /////////////

    public:
        Viewport viewport() const override;

        Scissors scissors() const override;

        void onViewResize(UINT width, UINT height) override;

        ////////////////
        // DrawObject //
        ////////////////

    protected:
        void onRendererUpdateObjectHelper(Renderer* rndr) override;
        void onRendererDrawD3d12ObjectHelper(Renderer* rndr) override;

        ////////////
        // Camera //
        ////////////

        //--------------------------------------------
        // View Matrix Raw Data
        //--------------------------------------------
        // Attention! After changing these parameters,
        // updateViewMatrix needs to be called to update the actual transformation matrix.

    public:
        XMFLOAT3 eyePos = { 0.0f, 0.0f, 0.0f };
        XMFLOAT3 eyeDir = { 0.0f, 0.0f, 1.0f };

        XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };

    protected:
        Viewport m_viewport = {};
        Scissors m_scissors = {};

        //--------------------------------------------
        // Projection Matrix Raw Data
        //--------------------------------------------
        // Attention! After changing these parameters,
        // updateProjMatrix needs to be called to update the actual transformation matrix.

    public:
        float fovAngleY = XM_PIDIV4; // in radians

        Optional<float> aspectRatio = {};

        // Returns viewport-width / viewport-height if aspectRatio is empty.
        float getAspectRatio() const;

        float nearZ = 1.0f, farZ = 1000.0f;

        //--------------------------------------------
        // Transformation Matrix
        //--------------------------------------------

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

        //--------------------------------------------
        // GPU/Shader Resources
        //--------------------------------------------

    public:
        UINT rootParamIndex = 0;

        // Indicates how many buffers need to be updated in the subsequent render passes.
        UINT dirtyFrameCount = FrameResource::g_bufferCount;

    protected:
        using Buffer = UniquePtr<ConstantBuffer>;
        using BufferArray = FrameResource::Array<Buffer>;

        BufferArray m_buffers = {};

    public:
        const BufferArray& buffers() const;
    };
}
