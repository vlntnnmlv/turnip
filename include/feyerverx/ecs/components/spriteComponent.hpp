// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/lrtb.hpp"
#include "feyerverx/texture.hpp"

namespace feyerverx::ecs {
struct SpriteComponent : IComponent {
    Texture &texture;
    LRTB patch;

    SpriteComponent(Texture &_Texture, LRTB _Patch = LRTB{0, 0, 0, 0});
};
} // namespace feyerverx::ecs
