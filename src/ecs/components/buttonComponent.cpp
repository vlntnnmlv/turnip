// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/components/buttonComponent.hpp"

namespace turnip::ecs {
ButtonComponent::ButtonComponent(std::function<void()> _OnClick, Entity _Image)
    : onClick(_OnClick), image(_Image) {};
} // namespace turnip::ecs
