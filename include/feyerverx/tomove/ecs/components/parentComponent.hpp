// Copyright 2025 Valentin Namleev

#pragma once

#include "turnip/ecs/component.hpp"
#include "turnip/ecs/entity.hpp"
#include "turnip/ecs/registry.hpp"

namespace feyerverx::ecs {
struct ParentComponent : IComponent {
    EntityID parent;

    ParentComponent(EntityID _Parent = NullEntityID);
};
} // namespace feyerverx::ecs
