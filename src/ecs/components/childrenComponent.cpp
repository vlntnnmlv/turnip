// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/components/childrenComponent.hpp"

namespace turnip::ecs {
ChildrenComponent::ChildrenComponent(std::vector<EntityID> _Children) : children(_Children) {}
} // namespace turnip::ecs
