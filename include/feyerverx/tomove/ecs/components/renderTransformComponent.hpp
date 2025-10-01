// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/component.hpp"
#include "./turnip/lrtb.hpp"

#include <Vector2.hpp>

namespace feyerverx::ecs {
struct RenderTransformComponent : IComponent {
    Vector2 offset{0, 0};
    LRTB rectOffset{0, 0, 0, 0};
    Vector2 scale{1, 1};

    RenderTransformComponent(Vector2 _Offset = {0, 0}, LRTB _RectOffset = {0, 0, 0, 0},
                             Vector2 _Scale = {0, 0});
};
} // namespace feyerverx::ecs
