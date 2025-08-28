// Copyright 2025 Valentin Namleev

#include "./turnip/colorUtils.hpp"

namespace turnip {
Color ColorUtils::GetColorShade(float _Gradient, Color _Color) {
    return Color{(unsigned char)((int)_Color.r * _Gradient),
                 (unsigned char)((int)_Color.g * _Gradient),
                 (unsigned char)((int)_Color.b * _Gradient), 255};
}
} // namespace turnip
