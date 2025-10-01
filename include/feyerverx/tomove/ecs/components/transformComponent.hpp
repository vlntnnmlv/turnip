// Copyright 2025 Valentin Namleev

#pragma once

#include <Rectangle.hpp>

#include "./turnip/ecs/component.hpp"
#include "./turnip/interpolated/rectlerp.hpp"

namespace feyerverx::ecs {
struct TransformComponent : IComponent {
    raylib::Rectangle rect;
    raylib::Rectangle worldRect;

    TransformComponent(Rectangle _Rect = Rectangle{0, 0, 0, 0});
};
} // namespace feyerverx::ecs
