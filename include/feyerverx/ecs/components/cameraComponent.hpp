// Copyright 2025 Valentin Namleev

#pragma once

#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "feyerverx/color.hpp"
#include "feyerverx/ecs/component.hpp"
#include "feyerverx/math/vector.hpp"
#include "feyerverx/rectangleOffset.hpp"

namespace feyerverx::ecs {

enum class CameraType { PERSPECTIVE, ORTHOGRAPHIC };

struct CameraComponent : IComponent {
    CameraType type;
    RectangleOffset viewport;
    RectangleOffset viewportScaled; // TODO: Remove this shitty shit
    float near;
    float far;
    Vector3f position{};
    Color color = {255, 255, 255, 255};

    CameraComponent(CameraType type, RectangleOffset viewport, float near, float far);
    void updateViewport(RectangleOffset _viewportScaled, Vector2f windowSize);
    void setView(const uint16_t viewID) const {
        if (type == CameraType::ORTHOGRAPHIC) {
            static float projectionMatrix[16];
            bx::mtxOrtho(projectionMatrix, viewport.left, viewport.right, viewport.bottom,
                         viewport.top, near, far, 0.0f, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(viewID, nullptr, projectionMatrix);
            bgfx::setViewRect(
                viewID, static_cast<uint16_t>(viewport.left), static_cast<uint16_t>(viewport.top),
                static_cast<uint16_t>(viewport.right), static_cast<uint16_t>(viewport.bottom));
        }
    }
};
} // namespace feyerverx