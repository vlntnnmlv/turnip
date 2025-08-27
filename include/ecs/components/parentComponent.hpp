// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"
#include "../registry.hpp"

namespace turnip::ecs {
struct ParentComponent : IComponent {
    EntityID parent;

    ParentComponent(EntityID _Parent = NullEntity);
};
} // namespace turnip::ecs
