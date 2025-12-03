// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/childrenComponent.hpp"

namespace feyerverx::ecs {
ChildrenComponent::ChildrenComponent(std::vector<Entity> children) : children(children) {}
} // namespace feyerverx::ecs
