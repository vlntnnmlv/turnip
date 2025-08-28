// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/component.hpp"
#include "./turnip/ecs/registry.hpp"

namespace turnip::ecs {
struct ParentComponent : IComponent {
    EntityID parent;

    ParentComponent(EntityID _Parent = NullEntity);
};
} // namespace turnip::ecs
