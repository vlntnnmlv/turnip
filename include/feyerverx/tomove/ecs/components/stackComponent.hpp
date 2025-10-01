// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/component.hpp"

namespace feyerverx::ecs {

enum class StackType { HORIZONTAL = 0, VERTICAL = 1 };

enum class StackContentType { START = 0, END = 1, CENTER = 2 };

struct StackComponent : IComponent {
    StackType type;
    StackContentType contentType;
    float spacing;

    StackComponent(StackType _Type, StackContentType _ContentType = StackContentType::CENTER,
                   float _Spacing = 0);
};
} // namespace feyerverx::ecs
