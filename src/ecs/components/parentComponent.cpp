// Copyright 2025 Valentin Namleev

#include "./ecs/components/parentComponent.hpp"

namespace turnip::ecs {
ParentComponent::ParentComponent(EntityID _Parent) : parent(_Parent) {}
} // namespace turnip::ecs
