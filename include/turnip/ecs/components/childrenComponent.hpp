// Copyright 2025 Valentin Namleev

#pragma once

#include <vector>

#include "./turnip/ecs/component.hpp"
#include "./turnip/ecs/registry.hpp"

namespace turnip::ecs {
struct ChildrenComponent : IComponent {
    std::vector<EntityID> children;

    ChildrenComponent(std::vector<EntityID> _Children = {});
};
} // namespace turnip::ecs
