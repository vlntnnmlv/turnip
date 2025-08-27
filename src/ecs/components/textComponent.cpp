// Copyright 2025 Valentin Namleev

#include "./ecs/components/textComponent.hpp"

namespace turnip::ecs {
TextComponent::TextComponent(const std::string &_Text, raylib::Font &_Font, float _FontSize,
                             float _Spacing)
    : text(_Text), font(_Font), fontSize(_FontSize), spacing(_Spacing) {}
} // namespace turnip::ecs
