// Copyright 2025 Valentin Namleev

#pragma once

#include "./sizeType.hpp"
#include <optional>

namespace turnip {
struct Size {
    SizeType axisX;
    SizeType axisY;
    float width;
    float height;
    float x;
    float y;
    std::optional<float> minWidth;
    std::optional<float> minHeight;
    std::optional<float> maxHeight;
    std::optional<float> maxWidth;
};
} // namespace turnip
