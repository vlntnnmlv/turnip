// Copyright 2025 Valentin Namleev

#pragma once

// #include "../../interpolated/interpolated.hpp"
#include "../component.hpp"
#include <Rectangle.hpp>

namespace turnip::ecs {
struct TransformComponent : IComponent {
    raylib::Rectangle rect;
    raylib::Rectangle worldRect;

    TransformComponent(Rectangle _Rect = Rectangle{0, 0, 0, 0}) : rect(_Rect) {}
};
} // namespace turnip::ecs
