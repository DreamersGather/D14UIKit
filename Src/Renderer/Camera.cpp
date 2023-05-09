#include "Common/Precompile.h"

#include "Renderer/Camera.h"

#include "Renderer/GpuBuffer.h"
#include "Renderer/Renderer.h"

namespace d14engine::renderer
{
    Camera::Camera(ID3D12Device* device)
    {
        for (auto& buffer : m_buffers)
        {
            buffer = std::make_unique<ConstantBuffer>(device, 1, sizeof(m_data));
        }
    }

    Camera::Viewport Camera::viewport() const
    {
        return m_viewport;
    }

    Camera::Scissors Camera::scissors() const
    {
        return m_scissors;
    }

    void Camera::onViewResize(UINT viewWidth, UINT viewHeight)
    {
        viewWidth = std::max(viewWidth, 1u);
        viewHeight = std::max(viewHeight, 1u);

        m_viewport.TopLeftX = 0.0f;
        m_viewport.TopLeftY = 0.0f;
        m_viewport.Width = (FLOAT)viewWidth;
        m_viewport.Height = (FLOAT)viewHeight;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;

        m_scissors.left = 0;
        m_scissors.right = viewWidth;
        m_scissors.top = 0;
        m_scissors.bottom = viewHeight;

        updateProjMatrix();
    }

    void Camera::onRendererUpdateObjectHelper(Renderer* rndr)
    {
        if (dirtyFrameCount > 0)
        {
            --dirtyFrameCount;
            m_buffers.at(rndr->currFrameIndex())->copyData(0, &m_data, sizeof(m_data));
        }
    }

    void Camera::onRendererDrawD3d12ObjectHelper(Renderer* rndr)
    {
        rndr->cmdList()->RSSetViewports(1, &m_viewport);
        rndr->cmdList()->RSSetScissorRects(1, &m_scissors);

        auto buffer = m_buffers.at(rndr->currFrameIndex())->resource();

        rndr->cmdList()->SetGraphicsRootConstantBufferView(
            rootParamIndex, buffer->GetGPUVirtualAddress());
    }

    float Camera::getAspectRatio() const
    {
        if (aspectRatio.has_value())
        {
            return aspectRatio.value();
        }
        else return m_viewport.Width / m_viewport.Height;
    }

    XMMATRIX Camera::viewMatrix() const
    {
        return XMLoadFloat4x4(&m_data.viewMatrix);
    }

    XMMATRIX Camera::projMatrix() const
    {
        return XMLoadFloat4x4(&m_data.projMatrix);
    }

    void Camera::updateViewMatrix()
    {
        auto eyePosition = XMLoadFloat3(&eyePos);
        auto eyeDirection = XMLoadFloat3(&eyeDir);
        auto upDirection = XMLoadFloat3(&up);

        XMStoreFloat4x4(&m_data.viewMatrix, XMMatrixLookToLH(eyePosition, eyeDirection, upDirection));
    }

    void Camera::updateProjMatrix()
    {
        XMStoreFloat4x4(&m_data.projMatrix, XMMatrixPerspectiveFovLH(fovAngleY, getAspectRatio(), nearZ, farZ));
    }

    const Camera::BufferArray& Camera::buffers() const
    {
        return m_buffers;
    }
}
