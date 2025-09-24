// Copyright 2025 Valentin Namleev

#pragma once

#include <Rectangle.hpp>

#include "./turnip/interpolated/interpolated.hpp"

namespace turnip {
struct Rectlerp {
    Interpolated<float> x;
    Interpolated<float> y;
    Interpolated<float> width;
    Interpolated<float> height;

    raylib::Rectangle Rect() const;
    void SetRect(raylib::Rectangle _Rect);
};
} // namespace turnip
