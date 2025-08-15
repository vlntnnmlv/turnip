// Copyright 2025 Valentin Namleev

#pragma once

#include "./interpolated/interpolated.hpp"
#include <Rectangle.hpp>

namespace turnip {
struct Rectlerp {
    Interpolated<float> x;
    Interpolated<float> y;
    Interpolated<float> width;
    Interpolated<float> height;

    raylib::Rectangle Rect() const { return raylib::Rectangle{x, y, width, height}; }
    void SetRect(raylib::Rectangle _Rect) {
        x = _Rect.x;
        y = _Rect.y;
        width = _Rect.width;
        height = _Rect.height;
    }
};
} // namespace turnip
