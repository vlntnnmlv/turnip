// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/textComponent.hpp"

namespace feyerverx::ecs {
TextComponent::TextComponent(const std::string &_Text, Font &_Font, float _FontSize, float _Spacing)
    : text(_Text), font(_Font), fontSize(_FontSize), fontSizeOriginal(_FontSize),
      spacing(_Spacing) {}
} // namespace feyerverx::ecs
