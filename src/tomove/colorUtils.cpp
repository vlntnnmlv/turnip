// Copyright 2025 Valentin Namleev

#include "./turnip/colorUtils.hpp"

namespace turnip {
raylib::Color ColorUtils::GetColorShade(float _Gradient, raylib::Color _Color) {
    return raylib::Color{(unsigned char)((int)_Color.r * _Gradient),
                         (unsigned char)((int)_Color.g * _Gradient),
                         (unsigned char)((int)_Color.b * _Gradient), 255};
}

// TODO: https://gist.github.com/dkaraush/65d19d61396f5f3cd8ba7d1b4b3c9432
raylib::Color ColorUtils::OKLCH2RGB(raylib::Color _OKLCH) {
    return raylib::Color{(unsigned char)((int)_OKLCH.r), (unsigned char)((int)_OKLCH.g),
                         (unsigned char)((int)_OKLCH.b), 255};
}
} // namespace turnip
