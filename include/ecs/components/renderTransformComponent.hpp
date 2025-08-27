// Copyright 2025 Valentin Namleev

#pragma once

#include "../../lrtb.hpp"
#include "../component.hpp"

#include <Vector2.hpp>

namespace turnip::ecs {
struct RenderTransformComponent : IComponent {
    Vector2 offset;
    LRTB rectOffset;

    RenderTransformComponent(Vector2 _Offset = {0, 0}, LRTB _RectOffset = {0, 0, 0, 0});
};
} // namespace turnip::ecs
