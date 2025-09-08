// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/components/parentComponent.hpp"

namespace turnip::ecs {
ParentComponent::ParentComponent(Entity _Parent) : parent(_Parent) {}
} // namespace turnip::ecs
