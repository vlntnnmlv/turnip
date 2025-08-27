// Copyright 2025 Valentin Namleev

#include "./ecs/components/updateComponent.hpp"

namespace turnip::ecs {
UpdateComponent::UpdateComponent(std::function<void(float)> _OnUpdate) : onUpdate(_OnUpdate) {}
} // namespace turnip::ecs
