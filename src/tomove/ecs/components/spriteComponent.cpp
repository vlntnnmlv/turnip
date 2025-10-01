// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/components/spriteComponent.hpp"

namespace feyerverx::ecs {
SpriteComponent::SpriteComponent(raylib::Texture2D &_Texture, LRTB _Patch)
    : texture(_Texture), patch(_Patch) {}
} // namespace feyerverx::ecs
