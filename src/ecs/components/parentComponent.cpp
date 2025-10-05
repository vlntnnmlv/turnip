// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/parentComponent.hpp"

namespace feyerverx::ecs {
ParentComponent::ParentComponent(EntityID _Parent) : parent(_Parent) {}
} // namespace feyerverx::ecs
