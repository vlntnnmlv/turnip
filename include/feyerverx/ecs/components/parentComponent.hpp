// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/ecs/entity.hpp"

#include "feyerverx/ecs/registry.hpp"

namespace feyerverx::ecs {
struct ParentComponent : IComponent {
    Entity parent;

    explicit ParentComponent(Entity parent = {});
};
} // namespace feyerverx::ecs
