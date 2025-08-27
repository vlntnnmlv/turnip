// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"
#include "../registry.hpp"
#include <functional>

namespace turnip::ecs {
struct ButtonComponent : IComponent {
    std::function<void()> onClick;

    EntityID image;

    bool hovered;
    bool pressed;

    ButtonComponent(std::function<void()> _OnClick, EntityID _Image);
};
} // namespace turnip::ecs
