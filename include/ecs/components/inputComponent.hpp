// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"

namespace turnip::ecs {
struct InputComponent : IComponent {
    bool hovered;
};
} // namespace turnip::ecs
