// Copyright 2025 Valentin Namleev

#include "./turnip/interpolated/rectlerp.hpp"

namespace feyerverx {

raylib::Rectangle Rectlerp::Rect() const { return raylib::Rectangle{x, y, width, height}; }
void Rectlerp::SetRect(raylib::Rectangle _Rect) {
    x = _Rect.x;
    y = _Rect.y;
    width = _Rect.width;
    height = _Rect.height;
}
} // namespace feyerverx
