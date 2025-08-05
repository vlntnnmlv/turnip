// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"
#include <raylib-cpp.hpp>
#include <string>

namespace turnip::ecs {
struct TextComponent : IComponent {
    std::string text;
    raylib::Font &font;
    float fontSize;
    float spacing;

    TextComponent(const std::string &_Text, raylib::Font &_Font, float _FontSize, float _Spacing)
        : text(_Text), font(_Font), fontSize(_FontSize), spacing(_Spacing) {}
};
} // namespace turnip::ecs
