// Copyright 2025 Valentin Namleev

#pragma once

#include <functional>

#include "turnip/ecs/component.hpp"
#include "turnip/ecs/entity.hpp"
#include "turnip/ecs/registry.hpp"

namespace feyerverx::ecs {
struct ButtonComponent : IComponent {
    std::function<void()> onClick;

    Entity image;

    bool hovered;
    bool pressed;

    ButtonComponent(std::function<void()> _OnClick, Entity _Image);
};
} // namespace feyerverx::ecs
