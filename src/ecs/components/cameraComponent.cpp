// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/cameraComponent.hpp"

namespace feyerverx::ecs {

CameraComponent::CameraComponent(const RectangleOffset viewport, const float near, const float far,
                                 const Vector3f position, const Vector3f lookingAt,
                                 const Vector3f up, const Color color)
    : viewport(viewport), viewportScaled(viewport), type(CameraType::ORTHOGRAPHIC), near(near),
      far(far), position(position), lookingAt(lookingAt), up(up), color(color) {}

CameraComponent::CameraComponent(const float FOV, const float near, const float far,
                                 const Vector3f position, const Vector3f lookingAt,
                                 const Vector3f up, const Color color)
    : FOV(FOV), type(CameraType::PERSPECTIVE), near(near), far(far), position(position),
      lookingAt(lookingAt), up(up), color(color) {}

void CameraComponent::updateViewport(const Vector2f windowSize) {
    viewport.left =
        viewportScaled.left <= 1.0f ? windowSize.x * viewportScaled.left : viewportScaled.left;
    viewport.right =
        viewportScaled.right <= 1.0f ? windowSize.x * viewportScaled.right : viewportScaled.right;
    viewport.top =
        viewportScaled.top <= 1.0f ? windowSize.y * viewportScaled.top : viewportScaled.top;
    viewport.bottom = viewportScaled.bottom <= 1.0f ? windowSize.y * viewportScaled.bottom
                                                    : viewportScaled.bottom;
}
} // namespace feyerverx