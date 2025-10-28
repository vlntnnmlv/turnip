// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/cameraComponent.hpp"

namespace feyerverx::ecs {
CameraComponent::CameraComponent(const CameraType type, const RectangleOffset viewport,
                                 const float near, const float far)
    : type(type), viewport(viewport), viewportScaled(viewport), near(near), far(far) {}

void CameraComponent::updateViewport(const RectangleOffset _viewportScaled,
                                     const Vector2f windowSize) {
    viewport.left =
        _viewportScaled.left <= 1.0f ? windowSize.x * _viewportScaled.left : _viewportScaled.left;
    viewport.right = _viewportScaled.right <= 1.0f ? windowSize.x * _viewportScaled.right
                                                   : _viewportScaled.right;
    viewport.top =
        _viewportScaled.top <= 1.0f ? windowSize.y * _viewportScaled.top : _viewportScaled.top;
    viewport.bottom = _viewportScaled.bottom <= 1.0f ? windowSize.y * _viewportScaled.bottom
                                                     : _viewportScaled.bottom;
}
} // namespace feyerverx