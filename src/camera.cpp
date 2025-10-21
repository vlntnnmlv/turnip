#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>

#include "feyerverx/camera.hpp"

#include "feyerverx/eventSink.hpp"
#include "feyerverx/logger.hpp"

namespace feyerverx {

ICamera::ICamera(const std::string &id, const RectangleOffset viewport, const float near,
                 const float far)
    : IIdentifiable(id), m_viewport(viewport), m_viewportScaled(viewport), m_near(near),
      m_far(far) {
    EventSink::OnWindowResized +=
        [this](const Vector2f size) -> void { updateViewport(m_viewportScaled, size); };
}

RectangleOffset ICamera::viewport() const noexcept { return m_viewport; }
float ICamera::near() const noexcept { return m_near; }
float ICamera::far() const noexcept { return m_far; }

void ICamera::updateViewport(const RectangleOffset viewportScaled, const Vector2f windowSize) {
    m_viewport.left =
        viewportScaled.left <= 1.0f ? windowSize.x * viewportScaled.left : viewportScaled.left;
    m_viewport.right =
        viewportScaled.right <= 1.0f ? windowSize.x * viewportScaled.right : viewportScaled.right;
    m_viewport.top =
        viewportScaled.top <= 1.0f ? windowSize.y * viewportScaled.top : viewportScaled.top;
    m_viewport.bottom = viewportScaled.bottom <= 1.0f ? windowSize.y * viewportScaled.bottom
                                                      : viewportScaled.bottom;
}

std::unique_ptr<ICamera> CameraOrthogonal::create(const RectangleOffset viewport,
                                                               const float near, const float far) {
    return std::unique_ptr<CameraOrthogonal>{new CameraOrthogonal{viewport, near, far}};
}

CameraOrthogonal::CameraOrthogonal(const RectangleOffset viewport, const float near,
                                   const float far)
    : ICamera("camera_orthogonal", viewport, near, far) {
    Logger::instance().log(LogLevel::Info, "Camera constructed!");
}

void CameraOrthogonal::setView(const uint16_t viewID) const {
    static float projectionMatrix[16];
    bx::mtxOrtho(projectionMatrix, viewport().left, viewport().right, viewport().bottom,
                 viewport().top, near(), far(), 0.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(viewID, nullptr, projectionMatrix);
    bgfx::setViewRect(
        viewID, static_cast<uint16_t>(viewport().left), static_cast<uint16_t>(viewport().top),
        static_cast<uint16_t>(viewport().right), static_cast<uint16_t>(viewport().bottom));
}

std::unique_ptr<ICamera> CameraPerspective::create(const RectangleOffset viewport,
                                                                Vector3f position, const float FOV,
                                                                const float near, const float far) {
    return std::unique_ptr<CameraPerspective>{
        new CameraPerspective{viewport, position, FOV, near, far}};
}

CameraPerspective::CameraPerspective(const RectangleOffset viewport, const Vector3f position,
                                     const float FOV, const float near, const float far)
    : ICamera("camera_perspective", viewport, near, far), m_FOV(FOV), m_eye(position) {
    updateVectors();

    EventSink::OnKeyDown += [this](SDL_Keycode key) -> void {
        static float speed = 0.1f;
        if (key == SDLK_W) {
            m_eye.z -= speed;
        }

        if (key == SDLK_A) {
            m_eye.x -= speed;
        }

        if (key == SDLK_S) {
            m_eye.z += speed;
        }

        if (key == SDLK_D) {
            m_eye.x += speed;
        }

        if (key == SDLK_K) {
            m_eye.y -= speed;
        }

        if (key == SDLK_L) {
            m_eye.y += speed;
        }

        if (key == SDLK_Z) {
            m_rotation.x += speed / 10;
        }

        if (key == SDLK_X) {
            m_rotation.y += speed / 10;
        }

        if (key == SDLK_C) {
            m_rotation.z += speed / 10;
        }

        if (key == SDLK_N) {
            speed -= 0.1f;
        }

        if (key == SDLK_M) {
            speed += 0.1f;
        }

        updateVectors();
    };

    Logger::instance().log(LogLevel::Info, "Camera constructed!");
}

void CameraPerspective::setView(const uint16_t viewID) const {
    static float projectionMatrix[16];
    bx::mtxProj(projectionMatrix, m_FOV, 1, near(), far(), bgfx::getCaps()->homogeneousDepth);

    static float viewMatrix[16];
    const bx::Vec3 _eye(m_eye.x, m_eye.y, m_eye.z);
    const bx::Vec3 _at(m_lookingAt.x, m_lookingAt.y, m_lookingAt.z);
    const bx::Vec3 _up(m_up.x, m_up.y, m_up.z);
    bx::mtxLookAt(viewMatrix, _eye, _at, _up);

    bgfx::setViewTransform(viewID, viewMatrix, projectionMatrix);
    bgfx::setViewRect(
        viewID, static_cast<uint16_t>(viewport().left), static_cast<uint16_t>(viewport().top),
        static_cast<uint16_t>(viewport().right), static_cast<uint16_t>(viewport().bottom));
}
}