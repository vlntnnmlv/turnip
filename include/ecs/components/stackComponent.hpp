// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"

namespace turnip::ecs {

enum StackType { HORIZONTAL = 0, VERTICAL = 1 };

enum StackContentType { START = 0, END = 1, CENTER = 2 };

struct StackComponent : IComponent {
  StackType type;
  StackContentType contentType;
  float spacing;

  StackComponent(StackType _Type, StackContentType _ContentType = CENTER,
                 float _Spacing = 0)
      : type(_Type), contentType(_ContentType), spacing(_Spacing) {}
};
} // namespace turnip::ecs
