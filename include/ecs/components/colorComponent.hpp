// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"
#include <raylib.h>

namespace turnip::ecs {
struct ColorComponent : IComponent {
    ::Color color;

    ColorComponent(Color _Color = {255, 255, 255, 255});
};
} // namespace turnip::ecs
