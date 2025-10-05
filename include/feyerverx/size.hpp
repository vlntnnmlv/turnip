// Copyright 2025 Valentin Namleev

#pragma once

#include <optional>

#include "feyerverx/sizeType.hpp"

namespace feyerverx {
struct Size {
    SizeType axisX = SizeType::FILL;
    SizeType axisY = SizeType::FILL;
    float width = 0;
    float height = 0;
    float x = 0;
    float y = 0;
    std::optional<float> minWidth = std::nullopt;
    std::optional<float> minHeight = std::nullopt;
    std::optional<float> maxHeight = std::nullopt;
    std::optional<float> maxWidth = std::nullopt;
};
} // namespace feyerverx
