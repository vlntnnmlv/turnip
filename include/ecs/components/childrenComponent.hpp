// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"
#include "../registry.hpp"
#include <vector>

namespace turnip::ecs {
struct ChildrenComponent : IComponent {
    std::vector<EntityID> children;

    ChildrenComponent(std::vector<EntityID> _Children = {});
};
} // namespace turnip::ecs
