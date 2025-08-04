// Copyright 2025 Valentin Namleev

#pragma once

#include <Rectangle.hpp>
#include <Vector2.hpp>

namespace turnip {
class RectangleUtils {
public:
  static Vector2 Center(const Rectangle &_Rect) {
    return Vector2{_Rect.x + _Rect.width / 2, _Rect.y + _Rect.height / 2};
  }

  static Rectangle Move(Rectangle &_Rect, Vector2 _Offset) {
    _Rect.x += _Offset.x;
    _Rect.y += _Offset.y;
    return _Rect;
  }

  static Rectangle Expand(Rectangle _Rect, LRTB _Offset) {
    _Rect.x -= _Offset.left;
    _Rect.y -= _Offset.top;
    _Rect.width += _Offset.left + _Offset.right;
    _Rect.height += _Offset.top + _Offset.bottom;
    return _Rect;
  }
};
} // namespace turnip
