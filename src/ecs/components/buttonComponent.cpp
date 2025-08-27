// Copyright 2025 Valentin Namleev

#include "./ecs/components/buttonComponent.hpp"

namespace turnip::ecs {
ButtonComponent::ButtonComponent(std::function<void()> _OnClick, EntityID _Image)
    : onClick(_OnClick), image(_Image) {};
} // namespace turnip::ecs
