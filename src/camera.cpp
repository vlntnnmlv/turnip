#include <bgfx/bgfx.h>
#include <bx/math.h>

#include "feyerverx/camera.hpp"

#include "feyerverx/logger.hpp"

namespace feyerverx {

std::unique_ptr<ICamera> CameraOrthogonal::createUniquePointer(float left, float right, float top,
                                                               float bottom, float near,
                                                               float far) {
    return std::unique_ptr<CameraOrthogonal>{
        new CameraOrthogonal{left, right, top, bottom, near, far}};
}

CameraOrthogonal::CameraOrthogonal(const float left, const float right, const float top,
                                   const float bottom, const float near, const float far)
    : m_left(left), m_right(right), m_top(top), m_bottom(bottom), m_near(near), m_far(far) {
    Logger::instance().log(LogLevel::Info, "Camera constructed!");
}

void CameraOrthogonal::setView() const {
    float ortho[16];
    bx::mtxOrtho(ortho, m_left, m_right, m_bottom, m_top, m_near, m_far, 0.0f, false);
    bgfx::setViewTransform(0, nullptr, ortho);
    bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(m_right), static_cast<uint16_t>(m_bottom));
}

void CameraOrthogonal::resizeView(const float width, const float height) {
    m_right = width;
    m_bottom = height;
}
}