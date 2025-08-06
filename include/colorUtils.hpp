// Copyright 2025 Valentin Namleev

#pragma once

#include <Color.hpp>

namespace turnip {
class ColorUtils {
public:
    static Color GetColorShade(float _Gradient, Color _Color = WHITE) {
        return Color{(unsigned char)((int)_Color.r * _Gradient),
                     (unsigned char)((int)_Color.g * _Gradient),
                     (unsigned char)((int)_Color.b * _Gradient), 255};
    }
};

} // namespace turnip
