// Copyright 2025 Valentin Namleev

#pragma once

#include <raylib-cpp.hpp>
#include <string>

#include "./turnip/ecs/component.hpp"

namespace feyerverx::ecs {
struct TextComponent : IComponent {
    std::string text;
    raylib::Font &font;
    float fontSize;
    float fontSizeOriginal;
    float spacing;

    TextComponent(const std::string &_Text, raylib::Font &_Font, float _FontSize, float _Spacing);
};
} // namespace feyerverx::ecs
