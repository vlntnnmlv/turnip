// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/parentComponent.hpp"

namespace feyerverx::ecs {
ParentComponent::ParentComponent(Entity parent) : parent(parent) {}
} // namespace feyerverx::ecs
