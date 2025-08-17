// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"

#include <functional>

namespace turnip::ecs {
struct UpdateComponent : IComponent {
    std::function<void(float)> onUpdate;

    UpdateComponent(std::function<void(float)> _OnUpdate) : onUpdate(_OnUpdate) {}
};
} // namespace turnip::ecs
