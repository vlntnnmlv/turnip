// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/component.hpp"

namespace feyerverx::ecs {
struct HoverComponent : IComponent {
    bool hovered;
};
} // namespace feyerverx::ecs
