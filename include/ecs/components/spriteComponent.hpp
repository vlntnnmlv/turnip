// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"
#include "./lrtb.hpp"

#include <Texture.hpp>

namespace turnip::ecs {
struct SpriteComponent : IComponent {
    raylib::Texture2D &texture;
    LRTB patch;

    SpriteComponent(raylib::Texture2D &_Texture, LRTB _Patch = LRTB{0, 0, 0, 0})
        : texture(_Texture), patch(_Patch) {}
};
} // namespace turnip::ecs
