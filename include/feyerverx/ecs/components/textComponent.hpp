// Copyright 2025 Valentin Namleev

#pragma once

#include <string>

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/font.hpp"

namespace feyerverx::ecs {
struct TextComponent : IComponent {
    std::string text;
    Font &font;
    float fontSize;
    float fontSizeOriginal;
    float spacing;

    TextComponent(const std::string &_Text, Font &_Font, float _FontSize, float _Spacing);
};
} // namespace feyerverx::ecs
