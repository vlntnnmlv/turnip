// Copyright 2025 Valentin Namleev

#pragma once

#include <functional>

#include "./turnip/ecs/component.hpp"

namespace feyerverx::ecs {
using UpdateCallback = std::function<void(float, float)>;

struct UpdateComponent : IComponent {
    UpdateCallback onUpdate;
    UpdateComponent(UpdateCallback _OnUpdate);
};
} // namespace feyerverx::ecs
