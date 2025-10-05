// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/color.hpp"
#include "feyerverx/ecs/component.hpp"

namespace feyerverx::ecs {
struct ColorComponent : IComponent {
    Color color;

    ColorComponent(Color _Color = {255, 255, 255, 255});
};
} // namespace feyerverx::ecs
