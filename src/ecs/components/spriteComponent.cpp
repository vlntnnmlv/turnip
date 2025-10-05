// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/spriteComponent.hpp"

namespace feyerverx::ecs {
SpriteComponent::SpriteComponent(Texture &_Texture, LRTB _Patch)
    : texture(_Texture), patch(_Patch) {}
} // namespace feyerverx::ecs
