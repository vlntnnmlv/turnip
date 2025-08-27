// Copyright 2025 Valentin Namleev

#pragma once

#include "../../interpolated/rectlerp.hpp"
#include "../component.hpp"

#include <Rectangle.hpp>

namespace turnip::ecs {
struct TransformComponent : IComponent {
    raylib::Rectangle rect;
    Rectlerp worldRect;

    TransformComponent(Rectangle _Rect = Rectangle{0, 0, 0, 0});
};
} // namespace turnip::ecs
