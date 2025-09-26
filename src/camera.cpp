#include <bgfx/bgfx.h>
#include <bx/math.h>

#include "turnip/camera.hpp"

namespace turnip {
CameraOrthogonal::CameraOrthogonal(float left, float right, float top, float bottom, float near,
                                   float far)
    : m_left(left), m_right(right), m_top(top), m_bottom(bottom), m_near(near), m_far(far) {}

void CameraOrthogonal::setView() const {
    float ortho[16];
    bx::mtxOrtho(ortho, m_left, m_right, m_bottom, m_top, m_near, m_far, 0.0f, false);
    bgfx::setViewTransform(0, nullptr, ortho);
    bgfx::setViewRect(0, 0, 0, uint16_t(m_right), uint16_t(m_bottom));
}

void CameraOrthogonal::resizeView(float width, float height) {
    m_right = width;
    m_bottom = height;
}
}