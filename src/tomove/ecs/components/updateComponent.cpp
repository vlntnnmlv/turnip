// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/components/updateComponent.hpp"

namespace feyerverx::ecs {
UpdateComponent::UpdateComponent(UpdateCallback _OnUpdate) : onUpdate(_OnUpdate) {}
} // namespace feyerverx::ecs
