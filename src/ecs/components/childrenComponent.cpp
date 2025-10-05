// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/childrenComponent.hpp"

namespace feyerverx::ecs {
ChildrenComponent::ChildrenComponent(std::vector<EntityID> _Children) : children(_Children) {}
} // namespace feyerverx::ecs
