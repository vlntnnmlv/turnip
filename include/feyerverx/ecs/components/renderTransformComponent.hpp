// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/math/vector.hpp"
#include "feyerverx/rectangleOffset.hpp"

namespace feyerverx::ecs {
struct RenderTransformComponent : IComponent {
    Vector2f offset{0.0f, 0.0f};
    RectangleOffset rectOffset{0, 0, 0, 0};
    Vector2f scale{1, 1};

    RenderTransformComponent(Vector2f _Offset = {0, 0}, RectangleOffset _RectOffset = {0, 0, 0, 0},
                             Vector2f _Scale = {0, 0});
};
} // namespace feyerverx::ecs
