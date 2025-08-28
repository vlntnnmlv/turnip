// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/components/spriteComponent.hpp"

namespace turnip::ecs {
SpriteComponent::SpriteComponent(raylib::Texture2D &_Texture, LRTB _Patch)
    : texture(_Texture), patch(_Patch) {}
} // namespace turnip::ecs
