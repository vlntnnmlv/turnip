// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/lrtb.hpp"
#include "feyerverx/math/vector.hpp"

namespace feyerverx::ecs {
struct RenderTransformComponent : IComponent {
    Vector2f offset{0.0f, 0.0f};
    LRTB rectOffset{0, 0, 0, 0};
    Vector2f scale{1, 1};

    RenderTransformComponent(Vector2f _Offset = {0, 0}, LRTB _RectOffset = {0, 0, 0, 0},
                             Vector2f _Scale = {0, 0});
};
} // namespace feyerverx::ecs
