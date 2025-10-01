// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/components/buttonComponent.hpp"

namespace feyerverx::ecs {
ButtonComponent::ButtonComponent(std::function<void()> _OnClick, Entity _Image)
    : onClick(_OnClick), image(_Image) {};
} // namespace feyerverx::ecs
