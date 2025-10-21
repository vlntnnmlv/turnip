// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/rectangleOffset.hpp"
#include "feyerverx/texture.hpp"

namespace feyerverx::ecs {
struct SpriteComponent : IComponent {
    Texture &texture;
    RectangleOffset patch;

    SpriteComponent(Texture &texture, RectangleOffset patch = RectangleOffset{0, 0, 0, 0});
};
} // namespace feyerverx::ecs
