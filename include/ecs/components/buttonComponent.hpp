// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"
#include <functional>

namespace turnip::ecs {
struct ButtonComponent : IComponent {
    std::function<void()> onClick;
};
} // namespace turnip::ecs
