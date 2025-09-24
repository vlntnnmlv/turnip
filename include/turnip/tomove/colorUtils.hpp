// Copyright 2025 Valentin Namleev

#pragma once

#include <Color.hpp>

namespace turnip {
class ColorUtils {
public:
    static raylib::Color GetColorShade(float _Gradient, raylib::Color _Color = WHITE);
    static raylib::Color OKLCH2RGB(raylib::Color _OKLCH);
};

} // namespace turnip
