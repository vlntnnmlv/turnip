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
    // ORTHOGRAPHIC
    RectangleOffset viewport;
    RectangleOffset viewportScaled;

    // PERSPECTIVE
    float FOV{};

    CameraType type;
    float near;
    float far;

    Vector3f position;
    Vector3f rotation{};
    Vector3f lookingAt;
    Vector3f up;
    Color color;

    explicit CameraComponent(RectangleOffset viewport, float near = 0, float far = 1000,
                             Vector3f position = {}, Vector3f lookingAt = {}, Vector3f up = {},
                             Color color = {255, 255, 255, 255});

    explicit CameraComponent(float FOV, float near = 0, float far = 1000, Vector3f position = {},
                             Vector3f lookingAt = {}, Vector3f up = {},
                             Color color = {255, 255, 255, 255});

    void updateViewport(Vector2f windowSize);
};
} // namespace feyerverx