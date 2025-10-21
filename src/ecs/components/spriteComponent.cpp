// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/spriteComponent.hpp"

namespace feyerverx::ecs {
SpriteComponent::SpriteComponent(Texture &texture, RectangleOffset patch)
    : texture(texture), patch(patch) {}
} // namespace feyerverx::ecs
