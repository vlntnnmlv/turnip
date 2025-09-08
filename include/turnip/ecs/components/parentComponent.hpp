// Copyright 2025 Valentin Namleev

#pragma once

#include "turnip/ecs/component.hpp"
#include "turnip/ecs/entity.hpp"
#include "turnip/ecs/registry.hpp"

namespace turnip::ecs {
struct ParentComponent : IComponent {
    Entity parent;

    ParentComponent(Entity _Parent = Entity{});
};
} // namespace turnip::ecs
