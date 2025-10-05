// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/component.hpp"

#include "feyerverx/ecs/registry.hpp"

namespace feyerverx::ecs {
struct ParentComponent : IComponent {
    EntityID parent;

    ParentComponent(EntityID _Parent = NullEntityID);
};
} // namespace feyerverx::ecs
