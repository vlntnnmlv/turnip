// Copyright 2025 Valentin Namleev

#pragma once

#include <raylib-cpp.hpp>

#include "./turnip/ecs/component.hpp"

namespace turnip::ecs {
struct ColorComponent : IComponent {
    ::Color color;

    ColorComponent(raylib::Color _Color = {255, 255, 255, 255});
};
} // namespace turnip::ecs
