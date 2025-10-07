// Copyright 2025 Valentin Namleev

#pragma once

#include <optional>
#include <vector>

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/ecs/entity.hpp"

namespace feyerverx::ecs {
struct ChildrenComponent : IComponent {
    std::optional<std::vector<Entity>> children = std::nullopt;

    explicit ChildrenComponent(std::vector<Entity> children = {});
};
} // namespace feyerverx::ecs
