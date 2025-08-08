// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"
#include <functional>

namespace turnip::ecs {
struct ButtonComponent : IComponent {
    std::function<void()> onClick;

    ButtonComponent(std::function<void()> _OnClick) : onClick(_OnClick) {};
};
} // namespace turnip::ecs
