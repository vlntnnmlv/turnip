// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/rectangle.hpp"

namespace feyerverx::ecs {
struct TransformComponent : IComponent {
    Rectangle rect;
    Rectangle worldRect;

    TransformComponent(Rectangle _Rect = Rectangle{0, 0, 0, 0});
};
} // namespace feyerverx::ecs
