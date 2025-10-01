// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/component.hpp"
#include "./turnip/lrtb.hpp"

#include <Texture.hpp>

namespace feyerverx::ecs {
struct SpriteComponent : IComponent {
    raylib::Texture2D &texture;
    LRTB patch;

    SpriteComponent(raylib::Texture2D &_Texture, LRTB _Patch = LRTB{0, 0, 0, 0});
};
} // namespace feyerverx::ecs
