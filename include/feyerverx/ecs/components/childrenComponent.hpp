// Copyright 2025 Valentin Namleev

#pragma once

#include <vector>

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/ecs/registry.hpp"

namespace feyerverx::ecs {
struct ChildrenComponent : IComponent {
    std::vector<EntityID> children;

    ChildrenComponent(std::vector<EntityID> _Children = {});
};
} // namespace feyerverx::ecs
