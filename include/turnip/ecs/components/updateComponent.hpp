// Copyright 2025 Valentin Namleev

#pragma once

#include <functional>

#include "./turnip/ecs/component.hpp"

namespace turnip::ecs {
struct UpdateComponent : IComponent {
    std::function<void(float)> onUpdate;

    UpdateComponent(std::function<void(float)> _OnUpdate);
};
} // namespace turnip::ecs
