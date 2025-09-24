// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/component.hpp"

namespace turnip::ecs {
struct HoverComponent : IComponent {
    bool hovered;
};
} // namespace turnip::ecs
