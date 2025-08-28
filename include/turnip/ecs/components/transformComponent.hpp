// Copyright 2025 Valentin Namleev

#pragma once

#include <Rectangle.hpp>

#include "./turnip/ecs/component.hpp"
#include "./turnip/interpolated/rectlerp.hpp"

namespace turnip::ecs {
struct TransformComponent : IComponent {
    raylib::Rectangle rect;
    Rectlerp worldRect;

    TransformComponent(Rectangle _Rect = Rectangle{0, 0, 0, 0});
};
} // namespace turnip::ecs
