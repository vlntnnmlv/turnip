// Copyright 2025 Valentin Namleev

#include "./turnip/rectangleUtils.hpp"

namespace turnip {
Vector2 RectangleUtils::Center(const Rectangle &_Rect) {
    return Vector2{_Rect.x + _Rect.width / 2, _Rect.y + _Rect.height / 2};
}

Vector2 RectangleUtils::Size(const Rectangle &_Rect) { return Vector2{_Rect.x, _Rect.y}; }

Rectangle RectangleUtils::Move(Rectangle &_Rect, Vector2 _Offset) {
    _Rect.x += _Offset.x;
    _Rect.y += _Offset.y;
    return _Rect;
}

Rectangle RectangleUtils::Expand(Rectangle _Rect, LRTB _Offset) {
    _Rect.x -= _Offset.left;
    _Rect.y -= _Offset.top;
    _Rect.width += _Offset.left + _Offset.right;
    _Rect.height += _Offset.top + _Offset.bottom;
    return _Rect;
}

inline raylib::Rectangle operator+(const raylib::Rectangle a, const raylib::Rectangle b) {
    return raylib::Rectangle(a.x + b.x, a.y + b.y, a.width + b.width, a.height + b.height);
}

inline raylib::Rectangle operator-(const raylib::Rectangle a, const raylib::Rectangle b) {
    return raylib::Rectangle(a.x - b.x, a.y - b.y, a.width - b.width, a.height - b.height);
}

inline raylib::Rectangle operator*(const raylib::Rectangle a, float b) {
    return raylib::Rectangle(a.x * b, a.y * b, a.width * b, a.height * b);
}

} // namespace turnip
