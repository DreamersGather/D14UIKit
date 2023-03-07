#include "Common/Precompile.h"

#include "Renderer/Camera.h"

#include "Renderer/GpuBuffer.h"
#include "Renderer/Renderer.h"

namespace d14engine::renderer
{
    Camera::Camera(Renderer* rndr)
    {
        onViewResize(rndr->getSceneHeight(), rndr->getSceneHeight());

        updateViewMatrix();
        // The projection matrix has already been updated in onViewResize.

        for (auto& buffer : m_buffers)
        {
            buffer = std::make_unique<ConstantBuffer>(rndr->d3d12Device(), 1, sizeof(m_data));
        }
    }

    bool Camera::isD3d12ObjectVisible() const
    {
        return m_visible;
    }

    void Camera::setD3d12ObjectVisible(bool value)
    {
        m_visible = value;
    }

    void Camera::onRendererUpdateObject(Renderer* rndr)
    {
        if (dirtyFrameCount > 0)
        {
            --dirtyFrameCount;
            m_buffers.at(rndr->currFrameIndex())->copyData(0, &m_data, sizeof(m_data));
        }
    }

    void Camera::onRendererDrawD3d12Object(Renderer* rndr)
    {
        rndr->cmdList()->SetGraphicsRootConstantBufferView(
            rootParamIndex, m_buffers.at(rndr->currFrameIndex())->resource()->GetGPUVirtualAddress());
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

    XMVECTOR Camera::position() const
    {
        return XMLoadFloat3(&m_position);
    }

    void Camera::setPosition(FXMVECTOR pos)
    {
        XMStoreFloat3(&m_position, pos);

        updateViewMatrix();
    }

    XMVECTOR Camera::right() const
    {
        return XMLoadFloat3(&m_right);
    }

    XMVECTOR Camera::up() const
    {
        return XMLoadFloat3(&m_up);
    }

    XMVECTOR Camera::look() const
    {
        return XMLoadFloat3(&m_look);
    }

    void Camera::setView(FXMVECTOR right, FXMVECTOR up, FXMVECTOR look)
    {
        XMStoreFloat3(&m_right, right);
        XMStoreFloat3(&m_up, up);
        XMStoreFloat3(&m_look, look);

        updateViewMatrix();
    }

    float Camera::fovAngleY() const
    {
        return m_fovAngleY;
    }

    void Camera::setFovAngleY(float rad)
    {
        m_fovAngleY = rad;

        updateProjMatrix();
    }

    Optional<float> Camera::aspectRatio() const
    {
        return m_aspectRatio;
    }

    void Camera::setAspectRatio(OptParam<float> value)
    {
        m_aspectRatio = value;
    }

    float Camera::getAspectRatio() const
    {
        if (m_aspectRatio.has_value())
        {
            return m_aspectRatio.value();
        }
        else return m_viewport.Width / m_viewport.Height;
    }

    float Camera::nearZ() const
    {
        return m_nearZ;
    }

    float Camera::farZ() const
    {
        return m_farZ;
    }

    void Camera::setNearFarZ(float nearZ, float farZ)
    {
        m_nearZ = nearZ;
        m_farZ = farZ;

        updateProjMatrix();
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
        m_data.viewMatrix(0, 0) = m_right.x;
        m_data.viewMatrix(0, 1) = m_up.x;
        m_data.viewMatrix(0, 2) = m_look.x;
        m_data.viewMatrix(0, 3) = 0.0f;

        m_data.viewMatrix(1, 0) = m_right.y;
        m_data.viewMatrix(1, 1) = m_up.y;
        m_data.viewMatrix(1, 2) = m_look.y;
        m_data.viewMatrix(1, 3) = 0.0f;

        m_data.viewMatrix(2, 0) = m_right.z;
        m_data.viewMatrix(2, 1) = m_up.z;
        m_data.viewMatrix(2, 2) = m_look.z;
        m_data.viewMatrix(2, 3) = 0.0f;

        XMVECTOR P = XMLoadFloat3(&m_position);

        XMVECTOR R = XMLoadFloat3(&m_right);
        XMVECTOR U = XMLoadFloat3(&m_up);
        XMVECTOR L = XMLoadFloat3(&m_look);

        m_data.viewMatrix(3, 0) = -XMVectorGetX(XMVector3Dot(P, R));
        m_data.viewMatrix(3, 1) = -XMVectorGetX(XMVector3Dot(P, U));
        m_data.viewMatrix(3, 2) = -XMVectorGetX(XMVector3Dot(P, L));
        m_data.viewMatrix(3, 3) = 1.0f;
    }

    void Camera::updateProjMatrix()
    {
        XMStoreFloat4x4(&m_data.projMatrix, XMMatrixPerspectiveFovLH(m_fovAngleY, getAspectRatio(), m_nearZ, m_farZ));
    }

    const Camera::BufferArray& Camera::buffers() const
    {
        return m_buffers;
    }
}
