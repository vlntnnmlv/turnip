// Copyright 2025 Valentin Namleev

#pragma once

#include <raylib-cpp.hpp>
#include <string>

#include "./turnip/ecs/component.hpp"

namespace turnip::ecs {
struct TextComponent : IComponent {
    std::string text;
    raylib::Font &font;
    float fontSize;
    float spacing;

    TextComponent(const std::string &_Text, raylib::Font &_Font, float _FontSize, float _Spacing);
};
} // namespace turnip::ecs
