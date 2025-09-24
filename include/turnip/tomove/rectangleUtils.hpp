// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/lrtb.hpp"
#include <Rectangle.hpp>
#include <Vector2.hpp>

namespace turnip {
class RectangleUtils {
public:
    static Vector2 Center(const Rectangle &_Rect);
    static Rectangle Move(Rectangle &_Rect, Vector2 _Offset);
    static Rectangle Expand(Rectangle _Rect, LRTB _Offset);
};
} // namespace turnip
